#include <CQLDriver/Common/Exceptions/AuthenticateException.hpp>
#include "../ResponseMessages/AuthenticateMessage.hpp"
#include "../Connection.hpp"
#include "./AllowAllAuthenticator.hpp"

namespace cql {
	/** Perform the authentication */
	seastar::future<> AllowAllAuthenticator::authenticate(
		const seastar::lw_shared_ptr<Connection>& connection,
		const ConnectionStream& stream) const {
		return connection->waitNextMessage(stream).then([] (auto message) {
			auto opCode = message->getHeader().getOpCode();
			if (opCode == MessageType::Authenticate) {
				auto authenticateMessage = std::move(message).template cast<AuthenticateMessage>();
				return seastar::make_exception_future(AuthenticateException(
					CQL_CODEINFO, "server required authentication:",
					authenticateMessage->getAuthenticatorClass().get()));
			} else if (opCode != MessageType::Ready) {
				return seastar::make_exception_future(AuthenticateException(
					CQL_CODEINFO, "unexpected authenticate response:", message->toString()));
			}
			// authenticate successful
			return seastar::make_ready_future();
		});
	}
}

