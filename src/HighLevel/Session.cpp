#include <core/do_with.hh>
#include <core/future-util.hh>
#include <CQLDriver/HighLevel/Session.hpp>
#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>
#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <CQLDriver/Common/Exceptions/ResponseErrorException.hpp>
#include "../LowLevel/RequestMessages/RequestMessageFactory.hpp"
#include "../LowLevel/RequestMessages/QueryMessage.hpp"
#include "../LowLevel/RequestMessages/ExecuteMessage.hpp"
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

		/** Used to store prepare results before handle them, to avoid pipeline interruption */
		struct PrepareResults {
			static void freeResources() { }
			void reset() { results.resize(0); }
			PrepareResults() : results() { }
			std::vector<std::pair<std::size_t, Object<ResponseMessageBase>>> results;
		};

		/** Used to control retry flow */
		class RetryFlow {
		public:
			/** Determinate should perform the retry for the specificed error code */
			static bool shouldRetryFor(ErrorCode errorCode) {
				// should not retry errors like SyntaxError
				std::size_t errorCodeValue = enumValue(errorCode);
				bool isUserError = errorCodeValue >= UserErrorCodeBegin &&
					errorCodeValue < UserErrorCodeEnd;
				return !isUserError;
			}

			/** Handle error message for "query" and "execute" */
			template <class... Args>
			seastar::future<Args...> handleErrorMessage(
				Object<ErrorMessage>&& errorMessage,
				Command& command,
				seastar::lw_shared_ptr<Connection>& connection) {
				auto errorCode = errorMessage->getErrorCode();
				if (errorCode == ErrorCode::UnPreparedQuery && unPreparedErrorCount_ == 0) {
					// evict cached prepare result
					if (*command.getNeedPrepare()) {
						auto& nodeConfiguration = connection->getNodeConfiguration();
						nodeConfiguration.getPreparedQueryId(command.getQuery()).resize(0);
					}
					++unPreparedErrorCount_;
					++maxRetries_;
				} else if (!shouldRetryFor(errorCode)) {
					// stop retry
					maxRetries_ = 0;
				}
				if (maxRetries_ > 0) {
					return seastar::make_exception_future<Args...>(RetryException());
				} else {
					return seastar::make_exception_future<Args...>(ResponseErrorException(
						CQL_CODEINFO, joinString("",
						errorCode, ": ", errorMessage->getErrorMessage().get(),
						", query: ", command.getQuery())));
				}
			}

			/** Handle error message for "batchExecute" */
			template <class... Args>
			seastar::future<Args...> handleErrorMessage(
				Object<ErrorMessage>&& errorMessage,
				BatchCommand& command,
				seastar::lw_shared_ptr<Connection>& connection) {
				auto errorCode = errorMessage->getErrorCode();
				if (errorCode == ErrorCode::UnPreparedQuery && unPreparedErrorCount_ == 0) {
					// evict cached prepare results
					auto& nodeConfiguration = connection->getNodeConfiguration();
					for (const auto& query : command.getQueries()) {
						if (*query.needPrepare) {
							nodeConfiguration.getPreparedQueryId(query.queryStr).resize(0);
						}
					}
					++unPreparedErrorCount_;
					++maxRetries_;
				} else if (!shouldRetryFor(errorCode)) {
					// stop retry
					maxRetries_ = 0;
				}
				if (maxRetries_ > 0) {
					return seastar::make_exception_future<Args...>(RetryException());
				} else {
					std::string allQueries;
					for (const auto& query : command.getQueries()) {
						allQueries.append(query.queryStr.get());
						allQueries.append("; ");
					}
					if (!allQueries.empty()) {
						allQueries.resize(allQueries.size()-1);
					}
					return seastar::make_exception_future<Args...>(ResponseErrorException(
						CQL_CODEINFO, joinString("",
						errorCode, ": ", errorMessage->getErrorMessage().get(),
						", queries: ", allQueries)));
				}
			}

			/** Handle unexpected message */
			template <class... Args>
			seastar::future<Args...> handleUnexpectMessage(
				Object<ResponseMessageBase>&& message,
				const char* requestType) {
				maxRetries_ = 0;
				return seastar::make_exception_future<Args...>(LogicException(
					CQL_CODEINFO, "unexpected response to", requestType, "message:",
					message->toString()));
			}

			/** Handle unexpected kind in result message */
			template <class... Args>
			seastar::future<Args...> handleUnexpectResultKind(
				Object<ResultMessage>&& resultMessage,
				const char* requestType) {
				maxRetries_ = 0;
				return seastar::make_exception_future<Args...>(LogicException(
					CQL_CODEINFO, "unexpected result kind to", requestType, "message:",
					resultMessage->toString()));
			}

			/** Determinate whether should retry or not */
			seastar::future<seastar::stop_iteration> determinate(std::exception_ptr&& ex) {
				if (maxRetries_ > 0) {
					--maxRetries_;
					return seastar::make_ready_future<
						seastar::stop_iteration>(seastar::stop_iteration::no);
				} else {
					return seastar::make_exception_future<
						seastar::stop_iteration>(std::move(ex));
				}
			}

			/** Constructor */
			explicit RetryFlow(std::size_t maxRetries) :
				maxRetries_(maxRetries), unPreparedErrorCount_(0) { }

		private:
			std::size_t maxRetries_;
			std::size_t unPreparedErrorCount_;
		};

		/** For those who use query to execute statements that produce no result */
		ResultSet getEmptyResultSet() {
			return ResultSet(makeObject<ResultSetData>(
				0, 0, "", seastar::temporary_buffer<char>(0), 0, 0));
		}

		/** Set default values in Command from SessionConfiguration */
		void setDefaultValues(
			const SessionConfiguration& sessionConfiguration,
			Command& command) {
			if (!command.getConsistency().has_value()) {
				command.setConsistency(sessionConfiguration.getDefaultConsistency());
			}
			if (!command.getNeedPrepare().has_value()) {
				command.prepareQuery(sessionConfiguration.getPrepareAllQueries());
			}
		}

		/** Set default values in BatchCommand from SessionConfiguration */
		void setDefaultValues(
			const SessionConfiguration& sessionConfiguration,
			BatchCommand& command) {
			if (!command.getConsistency().has_value()) {
				command.setConsistency(sessionConfiguration.getDefaultConsistency());
			}
			for (auto& query : command.getQueries()) {
				if (!query.needPrepare.has_value()) {
					query.needPrepare = sessionConfiguration.getPrepareAllQueries();
				}
			}
		}

		/**
		 * Prepare a single query for "query" and "execute".
		 * Return an EXECUTE message if the query is prepared,
		 * otherwise return a QUERY message.
		 */
		seastar::future<Object<RequestMessageBase>> prepareQuery(
			Command& command,
			RetryFlow& retryFlow,
			seastar::lw_shared_ptr<Connection>& connection,
			ConnectionStream& stream) {
			if (!*command.getNeedPrepare()) {
				// not require prepare
				auto queryMessage = RequestMessageFactory::makeRequestMessage<QueryMessage>();
				auto& queryParameters = queryMessage->getQueryParameters();
				queryParameters.setSkipMetadata(true);
				queryParameters.setCommandRef(command);
				return seastar::make_ready_future<Object<RequestMessageBase>>(
					std::move(queryMessage).cast<RequestMessageBase>());
			}
			auto& nodeConfiguration = connection->getNodeConfiguration();
			auto& logger = connection->getSessionConfiguration().getLogger();
			auto& preparedQueryId = nodeConfiguration.getPreparedQueryId(command.getQuery());
			if (!preparedQueryId.empty()) {
				// prepared on this node before
				if (logger->isEnabled(LogLevel::Debug)) {
					logger->log(LogLevel::Debug, "hit prepared cache:", command.getQuery().get());
				}
				auto executeMessage = RequestMessageFactory::makeRequestMessage<ExecuteMessage>();
				auto& queryParameters = executeMessage->getQueryParameters();
				queryParameters.setSkipMetadata(true);
				queryParameters.setCommandRef(command);
				executeMessage->getPreparedQueryId().set(preparedQueryId);
				return seastar::make_ready_future<Object<RequestMessageBase>>(
					std::move(executeMessage).cast<RequestMessageBase>());
			}
			// send PREPARE
			auto queryView = command.getQuery().get();
			auto prepareMessage = RequestMessageFactory::makeRequestMessage<PrepareMessage>();
			prepareMessage->getQuery().set(queryView.data(), queryView.size());
			if (logger->isEnabled(LogLevel::Debug)) {
				logger->log(LogLevel::Debug, "prepare:", queryView);
			}
			return connection->sendMessage(std::move(prepareMessage), stream).then([&connection, &stream] {
				// receive RESULT
				return connection->waitNextMessage(stream);
			}).then([&command, &retryFlow, &connection] (auto message) {
				// handle RESULT
				if (message->getHeader().getOpCode() == MessageType::Result) {
					auto resultMessage = std::move(message).template cast<ResultMessage>();
					if (resultMessage->getKind() == ResultKind::Prepared) {
						// cache prepare result to node
						auto& nodeConfiguration = connection->getNodeConfiguration();
						nodeConfiguration.getPreparedQueryId(command.getQuery()) = (
							resultMessage->getPreparedQueryId().get());
						// return execute message
						auto executeMessage = RequestMessageFactory::makeRequestMessage<ExecuteMessage>();
						auto& queryParameters = executeMessage->getQueryParameters();
						queryParameters.setSkipMetadata(true);
						queryParameters.setCommandRef(command);
						executeMessage->getPreparedQueryId().set(
							std::move(resultMessage->getPreparedQueryId().get()));
						return seastar::make_ready_future<Object<RequestMessageBase>>(
							std::move(executeMessage).cast<RequestMessageBase>());
					} else {
						return retryFlow.handleUnexpectResultKind<Object<RequestMessageBase>>(
							std::move(resultMessage), "PREPARE");
					}
				} else if (message->getHeader().getOpCode() == MessageType::Error) {
					auto errorMessage = std::move(message).template cast<ErrorMessage>();
					return retryFlow.handleErrorMessage<Object<RequestMessageBase>>(
						std::move(errorMessage), command, connection);
				} else {
					return retryFlow.handleUnexpectMessage<Object<RequestMessageBase>>(
						std::move(message), "PREPARE");
				}
			});
		}

		/**
		 * Prepare queries for "batchExecute".
		 * This function used pipeline feature of connection,
		 * notice the max pending messages setting from SessionConfiguration,
		 * if max pending messages is 100 and there 101 queries to prepare,
		 * this function may failure because of this limitation.
		 */
		seastar::future<> prepareQueries(
			BatchCommand& command,
			RetryFlow& retryFlow,
			seastar::lw_shared_ptr<Connection>& connection,
			ConnectionStream& stream,
			Object<BatchMessage>& batchMessage,
			Object<PrepareResults>& prepareResults) {
			auto& queries = command.getQueries();
			auto& nodeConfiguration = connection->getNodeConfiguration();
			auto& logger = connection->getSessionConfiguration().getLogger();
			seastar::future<> result = seastar::make_ready_future();
			// send PREPAREs
			for (std::size_t i = 0, j = queries.size(); i < j; ++i) {
				auto& query = queries[i];
				if (!*query.needPrepare) {
					continue;
				}
				auto& preparedQueryId = nodeConfiguration.getPreparedQueryId(query.queryStr);
				if (!preparedQueryId.empty()) {
					if (logger->isEnabled(LogLevel::Debug)) {
						logger->log(LogLevel::Debug, "hit prepared cache:", query.queryStr.get());
					}
					batchMessage->getPreparedQueryId(i) = preparedQueryId;
					continue; // use previous cached prepare result
				}
				auto queryView = query.queryStr.get();
				prepareResults->results.emplace_back(i, Object<ResponseMessageBase>());
				if (logger->isEnabled(LogLevel::Debug)) {
					logger->log(LogLevel::Debug, "prepare:", queryView);
				}
				result = result.then([&connection, &stream, queryView] {
					auto prepareMessage = RequestMessageFactory::makeRequestMessage<PrepareMessage>();
					prepareMessage->getQuery().set(queryView.data(), queryView.size());
					return connection->sendMessage(std::move(prepareMessage), stream);
				});
			}
			if (prepareResults->results.empty()) {
				return result; // all queries need prepare prepared
			}
			// receive RESULTs
			for (std::size_t k = 0, l = prepareResults->results.size(); k < l; ++k) {
				result = result.then([&connection, &stream] {
					// receive RESULT
					return connection->waitNextMessage(stream);
				}).then([&prepareResults, k] (auto message) {
					// store RESULT, don't handle it until all RESULTs received
					prepareResults->results[k].second = std::move(message);
				});
			}
			// handle RESULTs
			result = result.then([&command, &retryFlow, &connection, &batchMessage, &prepareResults] {
				auto& queries = command.getQueries();
				auto& nodeConfiguration = connection->getNodeConfiguration();
				for (auto& prepareResult : prepareResults->results) {
					auto& queryIndex = prepareResult.first;
					auto& message = prepareResult.second;
					if (message->getHeader().getOpCode() == MessageType::Result) {
						auto resultMessage = std::move(message).template cast<ResultMessage>();
						if (resultMessage->getKind() == ResultKind::Prepared) {
							// cache prepare result to node
							nodeConfiguration.getPreparedQueryId(queries[queryIndex].queryStr) = (
								resultMessage->getPreparedQueryId().get());
							// store prepared id in batch message
							batchMessage->getPreparedQueryId(queryIndex) = (
								std::move(resultMessage->getPreparedQueryId().get()));
						} else {
							return retryFlow.handleUnexpectResultKind(std::move(resultMessage), "PREPARE");
						}
					} else if (message->getHeader().getOpCode() == MessageType::Error) {
						auto errorMessage = std::move(message).template cast<ErrorMessage>();
						return retryFlow.handleErrorMessage(std::move(errorMessage), command, connection);
					} else {
						return retryFlow.handleUnexpectMessage(std::move(message), "PREPARE");
					}
				}
				return seastar::make_ready_future();
			});
			return result;
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
		return seastar::do_with(
			std::move(command),
			data_->connectionPool,
			RetryFlow(command.getMaxRetries()),
			seastar::lw_shared_ptr<Connection>(),
			ConnectionStream(),
			ResultSet(nullptr), [] (
			auto& command,
			auto& connectionPool,
			auto& retryFlow,
			auto& connection,
			auto& stream,
			auto& result) {
			// set default values in command
			setDefaultValues(connectionPool->getSessionConfiguration(), command);
			// start retry loop
			return seastar::repeat([
				&command,
				&connectionPool,
				&retryFlow,
				&connection,
				&stream,
				&result] {
				// get connection
				return connectionPool->getConnection().then(
					[&connection, &stream] (auto connectionVal, auto streamVal) {
					connection = std::move(connectionVal);
					stream = std::move(streamVal);
				}).then([&command, &retryFlow, &connection, &stream] {
					// prepare query
					return prepareQuery(command, retryFlow, connection, stream);
				}).then([&connection, &stream] (auto message) {
					// send QUERY or EXECUTE
					return connection->sendMessage(std::move(message), stream);
				}).then([&connection, &stream] {
					// receive RESULT
					return connection->waitNextMessage(stream);
				}).then([&command, &retryFlow, &connection, &result] (auto message) {
					// handle RESULT
					if (message->getHeader().getOpCode() == MessageType::Result) {
						auto resultMessage = std::move(message).template cast<ResultMessage>();
						if (resultMessage->getKind() == ResultKind::Rows) {
							result = std::move(resultMessage->getResultSet());
						} else {
							result = getEmptyResultSet();
						}
						return seastar::make_ready_future();
					}
					// handle ERROR
					if (message->getHeader().getOpCode() == MessageType::Error) {
						auto errorMessage = std::move(message).template cast<ErrorMessage>();
						return retryFlow.handleErrorMessage(std::move(errorMessage), command, connection);
					}
					// unexpected message type
					return retryFlow.handleUnexpectMessage(std::move(message), "QUERY");
				}).then([&connectionPool, &connection, &stream] {
					// query successful
					return seastar::stop_iteration::yes;
				}).handle_exception([&retryFlow] (std::exception_ptr ex) {
					// query failed
					return retryFlow.determinate(std::move(ex));
				}).finally([&connectionPool, &connection, &stream] {
					// return the connection
					if (connection.get() != nullptr && stream.isValid()) {
						connectionPool->returnConnection(std::move(connection), std::move(stream));
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
			return seastar::make_exception_future(
				LogicException(CQL_CODEINFO, "invalid command"));
		}
		return seastar::do_with(
			std::move(command),
			data_->connectionPool,
			RetryFlow(command.getMaxRetries()),
			seastar::lw_shared_ptr<Connection>(),
			ConnectionStream(), [] (
			auto& command,
			auto& connectionPool,
			auto& retryFlow,
			auto& connection,
			auto& stream) {
			// set default values in command
			setDefaultValues(connectionPool->getSessionConfiguration(), command);
			// start retry loop
			return seastar::repeat([
				&command,
				&connectionPool,
				&retryFlow,
				&connection,
				&stream] {
				// get connection
				return connectionPool->getConnection().then(
					[&connection, &stream] (auto connectionVal, auto streamVal) {
					connection = std::move(connectionVal);
					stream = std::move(streamVal);
				}).then([&command, &retryFlow, &connection, &stream] {
					// prepare query
					return prepareQuery(command, retryFlow, connection, stream);
				}).then([&connection, &stream] (auto message) {
					// send QUERY or EXECUTE
					return connection->sendMessage(std::move(message), stream);
				}).then([&connection, &stream] {
					// receive RESULT
					return connection->waitNextMessage(stream);
				}).then([&command, &retryFlow, &connection] (auto message) {
					// handle RESULT
					if (message->getHeader().getOpCode() == MessageType::Result) {
						return seastar::make_ready_future();
					}
					// handle ERROR
					if (message->getHeader().getOpCode() == MessageType::Error) {
						auto errorMessage = std::move(message).template cast<ErrorMessage>();
						return retryFlow.handleErrorMessage(std::move(errorMessage), command, connection);
					}
					// unexpected message type
					return retryFlow.handleUnexpectMessage(std::move(message), "QUERY");
				}).then([&connectionPool, &connection, &stream] {
					// execute successful
					return seastar::stop_iteration::yes;
				}).handle_exception([&retryFlow] (std::exception_ptr ex) {
					// execute failed
					return retryFlow.determinate(std::move(ex));
				}).finally([&connectionPool, &connection, &stream] {
					// return the connection
					if (connection.get() != nullptr && stream.isValid()) {
						connectionPool->returnConnection(std::move(connection), std::move(stream));
					}
				});
			});
		});
	}

	/** Perform multiple modification commands in batch */
	seastar::future<> Session::batchExecute(BatchCommand&& command) {
		if (!command.isValid()) {
			return seastar::make_exception_future(
				LogicException(CQL_CODEINFO, "invalid command"));
		} else if (command.getQueries().empty()) {
			// shortcut for empty batch
			return seastar::make_ready_future();
		}
		return seastar::do_with(
			std::move(command),
			data_->connectionPool,
			RetryFlow(command.getMaxRetries()),
			seastar::lw_shared_ptr<Connection>(),
			ConnectionStream(),
			Object<BatchMessage>(),
			Object<PrepareResults>(), [] (
			auto& command,
			auto& connectionPool,
			auto& retryFlow,
			auto& connection,
			auto& stream,
			auto& batchMessage,
			auto& prepareResults) {
			// set default values in batch command
			setDefaultValues(connectionPool->getSessionConfiguration(), command);
			// start retry loop
			return seastar::repeat([
				&command,
				&connectionPool,
				&retryFlow,
				&connection,
				&stream,
				&batchMessage,
				&prepareResults] {
				// get connection
				return connectionPool->getConnection().then(
					[&connection, &stream] (auto connectionVal, auto streamVal) {
					connection = std::move(connectionVal);
					stream = std::move(streamVal);
				}).then([&command, &retryFlow, &connection, &stream, &batchMessage, &prepareResults] {
					// prepare queries
					batchMessage = RequestMessageFactory::makeRequestMessage<BatchMessage>();
					batchMessage->getBatchParameters().setBatchCommandRef(command);
					prepareResults = makeObject<PrepareResults>();
					return prepareQueries(
						command, retryFlow, connection, stream, batchMessage, prepareResults);
				}).then([&connection, &stream, &batchMessage] {
					// send BATCH
					return connection->sendMessage(std::move(batchMessage), stream);
				}).then([&connection, &stream] {
					// receive RESULT
					return connection->waitNextMessage(stream);
				}).then([&command, &retryFlow, &connection] (auto message) {
					// handle RESULT
					if (message->getHeader().getOpCode() == MessageType::Result) {
						return seastar::make_ready_future();
					} else if (message->getHeader().getOpCode() == MessageType::Error) {
						auto errorMessage = std::move(message).template cast<ErrorMessage>();
						return retryFlow.handleErrorMessage(std::move(errorMessage), command, connection);
					} else {
						return retryFlow.handleUnexpectMessage(std::move(message), "BATCH");
					}
				}).then([] {
					// batch execute successful
					return seastar::stop_iteration::yes;
				}).handle_exception([&retryFlow] (std::exception_ptr ex) {
					// batch execute failed
					return retryFlow.determinate(std::move(ex));
				}).finally([&connectionPool, &connection, &stream] {
					// return the connection
					if (connection.get() != nullptr && stream.isValid()) {
						connectionPool->returnConnection(std::move(connection), std::move(stream));
					}
				});
			});
		});
	}

	/** Constructor */
	Session::Session(Object<SessionData>&& data) :
		data_(std::move(data)) { }
}

