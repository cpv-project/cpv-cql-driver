#include <core/do_with.hh>
#include <core/future-util.hh>
#include <CQLDriver/HighLevel/Session.hpp>
#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>
#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <CQLDriver/Common/Exceptions/ResponseErrorException.hpp>
#include "../LowLevel/RequestMessages/RequestMessageFactory.hpp"
#include "../LowLevel/RequestMessages/QueryMessage.hpp"
#include "../LowLevel/RequestMessages/PrepareMessage.hpp"
#include "../LowLevel/RequestMessages/BatchMessage.hpp"
#include "../LowLevel/ResponseMessages/ResultMessage.hpp"
#include "../LowLevel/ResponseMessages/ErrorMessage.hpp"
#include "../LowLevel/Connection.hpp"
#include "../Common/ResultSetData.hpp"
#include "../Common/BatchQueryData.hpp"
#include "./SessionData.hpp"

namespace cql {
	namespace {
		static const std::size_t UserErrorCodeBegin = 0x2000;
		static const std::size_t UserErrorCodeEnd = 0x3000;

		/** Simple exception only for retry */
		struct RetryException : public std::exception { };

		/** Should not retry errors like SyntaxError */
		bool shouldRetry(ErrorCode errorCode) {
			std::size_t errorCodeValue = enumValue(errorCode);
			bool isUserError = errorCodeValue >= UserErrorCodeBegin &&
				errorCodeValue < UserErrorCodeEnd;
			return !isUserError;
		}

		/** For those who use query to execute statements that produce no result */
		ResultSet getEmptyResultSet() {
			return ResultSet(makeObject<ResultSetData>(
				0, 0, "", seastar::temporary_buffer<char>(0), 0, 0));
		}

		/** Handle error message for query and execute */
		seastar::future<> handleErrorMessage(
			Object<ErrorMessage>&& errorMessage,
			Command& command,
			std::size_t& maxRetries) {
			auto errorCode = errorMessage->getErrorCode();
			if (!shouldRetry(errorCode)) {
				maxRetries = 0;
			}
			if (maxRetries > 0) {
				return seastar::make_exception_future<>(RetryException());
			} else {
				auto queryStr = command.getQuery();
				return seastar::make_exception_future<>(ResponseErrorException(
					CQL_CODEINFO, joinString("",
					errorCode, ": ", errorMessage->getErrorMessage().get(),
					", query: ", std::string(queryStr.first, queryStr.second))));
			}
		}

		/** Handle error message for batchExecute */
		seastar::future<> handleErrorMessage(
			Object<ErrorMessage>&& errorMessage,
			BatchCommand& command,
			std::size_t& maxRetries) {
			auto errorCode = errorMessage->getErrorCode();
			if (!shouldRetry(errorCode)) {
				maxRetries = 0;
			}
			if (maxRetries > 0) {
				return seastar::make_exception_future<>(RetryException());
			} else {
				std::string allQueries;
				for (const auto& query : command.getQueries()) {
					auto queryStr = query.getQuery();
					allQueries.append(queryStr.first, queryStr.second);
					allQueries.append("; ");
				}
				if (!allQueries.empty()) {
					allQueries.resize(allQueries.size()-1);
				}
				return seastar::make_exception_future<>(ResponseErrorException(
					CQL_CODEINFO, joinString("",
					errorCode, ": ", errorMessage->getErrorMessage().get(),
					", queries: ", allQueries)));
			}
		}

		/** Handle unexpected message */
		seastar::future<> handleUnexpectMessage(
			Object<ResponseMessageBase>&& message,
			const char* requestType,
			std::size_t& maxRetries) {
			maxRetries = 0;
			return seastar::make_exception_future<>(LogicException(
				CQL_CODEINFO, "unexpected response to", requestType, "message:",
				message->toString()));
		}

		/** Prepare queries for batchExecute */
		seastar::future<> prepareQueries(
			BatchCommand& command,
			std::size_t& maxRetries,
			seastar::lw_shared_ptr<Connection>& connection,
			ConnectionStream& stream,
			Object<BatchMessage>& batchMessage,
			std::size_t& prepareIndex) {
			return seastar::repeat(
				[&command, &maxRetries, &connection, &stream, &batchMessage, &prepareIndex] {
				auto& queries = command.getQueries();
				if (prepareIndex >= queries.size()) {
					// all queries prepared
					return seastar::make_ready_future<
						seastar::stop_iteration>(seastar::stop_iteration::yes);
				}
				auto thisIndex = prepareIndex++;
				auto& query = queries[thisIndex];
				if (!query.needPrepare) {
					// prepare next query
					return seastar::make_ready_future<
						seastar::stop_iteration>(seastar::stop_iteration::no);
				}
				// send PREPARE
				auto queryStr = query.getQuery();
				auto prepareMessage = RequestMessageFactory::makeRequestMessage<PrepareMessage>();
				prepareMessage->getQuery().set(queryStr.first, queryStr.second);
				return connection->sendMessage(std::move(prepareMessage), stream).then(
					[&connection, &stream] {
					// receive RESULT
					return connection->waitNextMessage(stream);
				}).then([&command, &maxRetries, &batchMessage, thisIndex] (auto message) {
					// handle RESULT
					if (message->getHeader().getOpCode() == MessageType::Result) {
						auto resultMessage = std::move(message).template cast<ResultMessage>();
						if (resultMessage->getKind() == ResultKind::Prepared) {
							batchMessage->getPreparedQueryId(thisIndex) = (
								std::move(resultMessage->getPreparedQueryId().get()));
							return seastar::make_ready_future<>();
						} else {
							maxRetries = 0;
							return seastar::make_exception_future<>(LogicException(
								CQL_CODEINFO, "unexpected result kind to prepare request",
								resultMessage->toString()));
						}
					}
					// handle ERROR
					if (message->getHeader().getOpCode() == MessageType::Error) {
						auto errorMessage = std::move(message).template cast<ErrorMessage>();
						return handleErrorMessage(std::move(errorMessage), command, maxRetries);
					}
					// unexpected message type
					return handleUnexpectMessage(std::move(message), "QUERY", maxRetries);
				}).then([] {
					// prepare next query
					return seastar::stop_iteration::no;
				});
			});
		}
	}

	/** Check whether this is a valid session (will be false if moved) */
	bool Session::isValid() const {
		return data_ != nullptr;
	}

	/** Execute the query command and return the result set */
	seastar::future<ResultSet> Session::query(Command&& command) {
		if (!command.isValid()) {
			return seastar::make_exception_future<ResultSet>(
				LogicException(CQL_CODEINFO, "invalid command"));
		}
		std::size_t maxRetries = command.getMaxRetries();
		return seastar::do_with(
			std::move(command), data_->connectionPool, maxRetries, ResultSet(nullptr),
			[] (auto& command, auto& connectionPool, auto& maxRetries, auto& result) {
			return seastar::repeat([&command, &connectionPool, &maxRetries, &result] {
				// get connection
				return connectionPool->getConnection().then(
					[&command, &maxRetries, &result] (auto connection, auto stream) {
					// send QUERY
					auto queryMessage = RequestMessageFactory::makeRequestMessage<QueryMessage>();
					auto& queryParameters = queryMessage->getQueryParameters();
					queryParameters.setSkipMetadata(true);
					queryParameters.setCommandRef(command);
					return connection->sendMessage(std::move(queryMessage), stream).then(
						[connection=std::move(connection), stream=std::move(stream)] {
						// receive RESULT
						return connection->waitNextMessage(stream);
					}).then([&command, &maxRetries, &result] (auto message) {
						// handle RESULT
						if (message->getHeader().getOpCode() == MessageType::Result) {
							auto resultMessage = std::move(message).template cast<ResultMessage>();
							if (resultMessage->getKind() == ResultKind::Rows) {
								result = std::move(resultMessage->getResultSet());
							} else {
								result = getEmptyResultSet();
							}
							return seastar::make_ready_future<>();
						}
						// handle ERROR
						if (message->getHeader().getOpCode() == MessageType::Error) {
							auto errorMessage = std::move(message).template cast<ErrorMessage>();
							return handleErrorMessage(std::move(errorMessage), command, maxRetries);
						}
						// unexpected message type
						return handleUnexpectMessage(std::move(message), "QUERY", maxRetries);
					});
				}).then([] {
					// query successful
					return seastar::stop_iteration::yes;
				}).handle_exception([&maxRetries] (std::exception_ptr ex) {
					// query failed
					if (maxRetries > 0) {
						--maxRetries;
						return seastar::make_ready_future<
							seastar::stop_iteration>(seastar::stop_iteration::no);
					} else {
						return seastar::make_exception_future<
							seastar::stop_iteration>(std::move(ex));
					}
				});
			}).then([&result] {
				if (!result.isValid()) {
					// just an assertion
					throw LogicException(CQL_CODEINFO, "returning invalid result");
				}
				return std::move(result);
			});
		});
	}

	/** Execute a single modification command */
	seastar::future<> Session::execute(Command&& command) {
		if (!command.isValid()) {
			return seastar::make_exception_future<>(
				LogicException(CQL_CODEINFO, "invalid command"));
		}
		std::size_t maxRetries = command.getMaxRetries();
		return seastar::do_with(
			std::move(command), data_->connectionPool, maxRetries,
			[] (auto& command, auto& connectionPool, auto& maxRetries) {
			return seastar::repeat([&command, &connectionPool, &maxRetries] {
				// get connection
				return connectionPool->getConnection().then(
					[&command, &maxRetries] (auto connection, auto stream) {
					// send QUERY
					auto queryMessage = RequestMessageFactory::makeRequestMessage<QueryMessage>();
					auto& queryParameters = queryMessage->getQueryParameters();
					queryParameters.setSkipMetadata(true);
					queryParameters.setCommandRef(command);
					return connection->sendMessage(std::move(queryMessage), stream).then(
						[connection=std::move(connection), stream=std::move(stream)] {
						// receive RESULT
						return connection->waitNextMessage(stream);
					}).then([&command, &maxRetries] (auto message) {
						// handle RESULT
						if (message->getHeader().getOpCode() == MessageType::Result) {
							return seastar::make_ready_future<>();
						}
						// handle ERROR
						if (message->getHeader().getOpCode() == MessageType::Error) {
							auto errorMessage = std::move(message).template cast<ErrorMessage>();
							return handleErrorMessage(std::move(errorMessage), command, maxRetries);
						}
						// unexpected message type
						return handleUnexpectMessage(std::move(message), "QUERY", maxRetries);
					});
				}).then([] {
					// execute successful
					return seastar::stop_iteration::yes;
				}).handle_exception([&maxRetries] (std::exception_ptr ex) {
					// execute failed
					if (maxRetries > 0) {
						--maxRetries;
						return seastar::make_ready_future<
							seastar::stop_iteration>(seastar::stop_iteration::no);
					} else {
						return seastar::make_exception_future<
							seastar::stop_iteration>(std::move(ex));
					}
				});
			});
		});
	}

	/** Perform multiple modification commands in batch */
	seastar::future<> Session::batchExecute(BatchCommand&& command) {
		if (!command.isValid()) {
			return seastar::make_exception_future<>(
				LogicException(CQL_CODEINFO, "invalid command"));
		} else if (command.getQueries().empty()) {
			// shortcut for empty batch
			return seastar::make_ready_future<>();
		}
		std::size_t maxRetries = command.getMaxRetries();
		return seastar::do_with(
			std::move(command), data_->connectionPool, maxRetries,
			[] (auto& command, auto& connectionPool, auto& maxRetries) {
			return seastar::repeat([&command, &connectionPool, &maxRetries] {
				// get connection
				return connectionPool->getConnection().then(
					[&command, &maxRetries] (auto connection, auto stream) {
					auto batchMessage = RequestMessageFactory::makeRequestMessage<BatchMessage>();
					batchMessage->getBatchParameters().setBatchCommandRef(command);
					// prepare queries
					std::size_t prepareIndex = 0;
					return seastar::do_with(
						std::move(connection), std::move(stream), std::move(batchMessage), prepareIndex,
						[&command, &maxRetries]
						(auto& connection, auto& stream, auto& batchMessage, auto& prepareIndex) {
						return prepareQueries(
							command, maxRetries, connection, stream, batchMessage, prepareIndex).then(
							[&connection, &stream, &batchMessage] {
							// send BATCH
							return connection->sendMessage(std::move(batchMessage), stream);
						}).then([&connection, &stream] {
							// receive RESULT
							return connection->waitNextMessage(stream);
						}).then([&command, &maxRetries] (auto message) {
							// handle RESULT
							if (message->getHeader().getOpCode() == MessageType::Result) {
								return seastar::make_ready_future<>();
							}
							// handle ERROR
							if (message->getHeader().getOpCode() == MessageType::Error) {
								auto errorMessage = std::move(message).template cast<ErrorMessage>();
								return handleErrorMessage(std::move(errorMessage), command, maxRetries);
							}
							// unexpected message type
							return handleUnexpectMessage(std::move(message), "BATCH", maxRetries);
						});
					});
				}).then([] {
					// batch execute successful
					return seastar::stop_iteration::yes;
				}).handle_exception([&maxRetries] (std::exception_ptr ex) {
					// batch execute failed
					if (maxRetries > 0) {
						--maxRetries;
						return seastar::make_ready_future<
							seastar::stop_iteration>(seastar::stop_iteration::no);
					} else {
						return seastar::make_exception_future<
							seastar::stop_iteration>(std::move(ex));
					}
				});
			});
		});
	}

	/** Constructor */
	Session::Session(Object<SessionData>&& data) :
		data_(std::move(data)) { }
}

