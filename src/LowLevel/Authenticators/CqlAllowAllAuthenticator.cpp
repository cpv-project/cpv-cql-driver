#include <CqlDriver/Common/Exceptions/CqlAuthenticateException.hpp>
#include "../ResponseMessages/CqlAuthenticateMessage.hpp"
#include "../CqlConnection.hpp"
#include "CqlAllowAllAuthenticator.hpp"

namespace cql {
	/** Perform the authentication */
	seastar::future<> CqlAllowAllAuthenticator::authenticate(
		const seastar::lw_shared_ptr<CqlConnection>& connection,
		const CqlConnectionStream& stream) const {
		return connection->waitNextMessage(stream).then([] (auto message) {
			auto opCode = message->getHeader().getOpCode();
			if (opCode == CqlMessageType::Authenticate) {
				CqlObject<CqlAuthenticateMessage> authenticateMessage(std::move(message));
				return seastar::make_exception_future(CqlAuthenticateException(
					CQL_CODEINFO, "server required authentication:",
					authenticateMessage->getAuthenticatorClass().get()));
			} else if (opCode != CqlMessageType::Ready) {
				return seastar::make_exception_future(CqlAuthenticateException(
					CQL_CODEINFO, "unexcepted authenticate response:", message->toString()));
			}
			// authenticate successful
			return seastar::make_ready_future();
		});
	}
}

