#include <CqlDriver/Common/CqlCommonDefinitions.hpp>
#include <CqlDriver/Common/Exceptions/CqlAuthenticateException.hpp>
#include "../RequestMessages/CqlRequestMessageFactory.hpp"
#include "../RequestMessages/CqlAuthResponseMessage.hpp"
#include "../ResponseMessages/CqlAuthenticateMessage.hpp"
#include "../CqlConnection.hpp"
#include "CqlPasswordAuthenticator.hpp"

namespace cql {
	/** Perform the authentication */
	seastar::future<> CqlPasswordAuthenticator::authenticate(
		const seastar::lw_shared_ptr<CqlConnection>& connection,
		const CqlConnectionStream& stream) const {
		return connection->waitNextMessage(stream).then([connection, &stream] (auto message) {
			auto opCode = message->getHeader().getOpCode();
			if (opCode == CqlMessageType::Ready) {
				return seastar::make_exception_future(CqlAuthenticateException(
					CQL_CODEINFO, "server didn't require authentication"));
			} else if (opCode != CqlMessageType::Authenticate) {
				return seastar::make_exception_future(CqlAuthenticateException(
					CQL_CODEINFO, "unexcepted authenticate response:", message->toString()));
			}
			CqlObject<CqlAuthenticateMessage> authenticateMessage(std::move(message));
			auto& authenticatorClass = authenticateMessage->getAuthenticatorClass().get();
			if (authenticatorClass != CqlAuthenticatorClasses::PasswordAuthenticator) {
				return seastar::make_exception_future(CqlAuthenticateException(
					CQL_CODEINFO, "authenticator not matched:", authenticatorClass));
			}
			auto authResponseMessage = CqlRequestMessageFactory::makeRequestMessage<CqlAuthResponseMessage>();
			authResponseMessage->getToken().set(connection->getNodeConfiguration().getAuthenticatorData());
			return connection->sendMessage(std::move(authResponseMessage), stream);
		}).then([connection, &stream] {
			return connection->waitNextMessage(stream);
		}).then([connection, &stream] (auto message) {
			auto opCode = message->getHeader().getOpCode();
			if (opCode != CqlMessageType::AuthSuccess) {
				return seastar::make_exception_future(CqlAuthenticateException(
					CQL_CODEINFO, "authenticate with password failed:", message->toString()));
			}
			// authenticate successful
			return seastar::make_ready_future();
		});
	}
}

