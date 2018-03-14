#include <core/do_with.hh>
#include <core/future-util.hh>
#include <CQLDriver/HighLevel/Session.hpp>
#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>
#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <CQLDriver/Common/Exceptions/ResponseErrorException.hpp>
#include "../LowLevel/RequestMessages/RequestMessageFactory.hpp"
#include "../LowLevel/RequestMessages/QueryMessage.hpp"
#include "../LowLevel/ResponseMessages/ResultMessage.hpp"
#include "../LowLevel/ResponseMessages/ErrorMessage.hpp"
#include "../LowLevel/Connection.hpp"
#include "../Common/ResultSetData.hpp"
#include "./SessionData.hpp"

namespace cql {
	namespace {
		static const std::size_t UserErrorCodeBegin = 0x2000;
		static const std::size_t UserErrorCodeEnd = 0x3000;

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
					}).then([&maxRetries, &result] (auto message) {
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
							auto errorCode = errorMessage->getErrorCode();
							if (!shouldRetry(errorCode)) {
								maxRetries = 0;
							}
							return seastar::make_exception_future<>(ResponseErrorException(
								CQL_CODEINFO, joinString("", errorCode, ":"),
								errorMessage->getErrorMessage().get()));
						}
						// unexpected message type
						maxRetries = 0;
						return seastar::make_exception_future<>(LogicException(
							CQL_CODEINFO, "unexpected response QUERY message:", message->toString()));
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
		throw NotImplementedException(CQL_CODEINFO);
	}

	/** Perform multiple modification commands in batch */
	seastar::future<> Session::batchExecute(BatchCommand command) {
		throw NotImplementedException(CQL_CODEINFO);
	}

	/** Constructor */
	Session::Session(Object<SessionData>&& data) :
		data_(std::move(data)) { }
}

