#include <CQLDriver/Common/CommonDefinitions.hpp>
#include <CQLDriver/Common/Exceptions/AuthenticateException.hpp>
#include "../RequestMessages/RequestMessageFactory.hpp"
#include "../RequestMessages/AuthResponseMessage.hpp"
#include "../ResponseMessages/AuthenticateMessage.hpp"
#include "../Connection.hpp"
#include "./PasswordAuthenticator.hpp"

namespace cql {
	/** Perform the authentication */
	seastar::future<> PasswordAuthenticator::authenticate(
		const seastar::lw_shared_ptr<Connection>& connection,
		const ConnectionStream& stream) const {
		return connection->waitNextMessage(stream).then([connection, &stream] (auto message) {
			auto opCode = message->getHeader().getOpCode();
			if (opCode == MessageType::Ready) {
				return seastar::make_exception_future(AuthenticateException(
					CQL_CODEINFO, "server didn't require authentication"));
			} else if (opCode != MessageType::Authenticate) {
				return seastar::make_exception_future(AuthenticateException(
					CQL_CODEINFO, "unexcepted authenticate response:", message->toString()));
			}
			auto authenticateMessage = std::move(message).template cast<AuthenticateMessage>();
			auto& authenticatorClass = authenticateMessage->getAuthenticatorClass().get();
			if (authenticatorClass != AuthenticatorClasses::PasswordAuthenticator) {
				return seastar::make_exception_future(AuthenticateException(
					CQL_CODEINFO, "authenticator not matched:", authenticatorClass));
			}
			auto authResponseMessage = RequestMessageFactory::makeRequestMessage<AuthResponseMessage>();
			authResponseMessage->getToken().set(connection->getNodeConfiguration().getAuthenticatorData());
			return connection->sendMessage(std::move(authResponseMessage), stream);
		}).then([connection, &stream] {
			return connection->waitNextMessage(stream);
		}).then([connection, &stream] (auto message) {
			auto opCode = message->getHeader().getOpCode();
			if (opCode != MessageType::AuthSuccess) {
				return seastar::make_exception_future(AuthenticateException(
					CQL_CODEINFO, "authenticate with password failed:", message->toString()));
			}
			// authenticate successful
			return seastar::make_ready_future();
		});
	}
}

