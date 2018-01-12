#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include "CqlResponseMessageFactory.hpp"
#include "CqlResponseMessageBase.hpp"
#include "CqlAuthChallengeMessage.hpp"
#include "CqlAuthenticateMessage.hpp"
#include "CqlAuthSuccessMessage.hpp"
#include "CqlErrorMessage.hpp"
#include "CqlEventMessage.hpp"
#include "CqlReadyMessage.hpp"
#include "CqlResultMessage.hpp"
#include "CqlSupportedMessage.hpp"

namespace cql {
	namespace CqlResponseMessageFactory {
		/** Creates a new response message instance with header set */
		CqlObject<CqlResponseMessageBase> makeResponseMessage(CqlMessageHeader&& header) {
			// order by frequency
			// RESULT > ERROR > EVENT > READY > SUPPORTED > AUTHENTICATE > AUTH_SUCCESS > AUTH_CHALLENGE
			CqlMessageType type = header.getOpCode();
			if (type == CqlMessageType::Result) {
				return makeObject<CqlResultMessage>(std::move(header));
			} else if (type == CqlMessageType::Error) {
				return makeObject<CqlErrorMessage>(std::move(header));
			} else if (type == CqlMessageType::Event) {
				return makeObject<CqlEventMessage>(std::move(header));
			} else if (type == CqlMessageType::Ready) {
				return makeObject<CqlReadyMessage>(std::move(header));
			} else if (type == CqlMessageType::Supported) {
				return makeObject<CqlSupportedMessage>(std::move(header));
			} else if (type == CqlMessageType::Authenticate) {
				return makeObject<CqlAuthenticateMessage>(std::move(header));
			} else if (type == CqlMessageType::AuthSuccess) {
				return makeObject<CqlAuthSuccessMessage>(std::move(header));
			} else if (type == CqlMessageType::AuthChallenge) {
				return makeObject<CqlAuthChallengeMessage>(std::move(header));
			} else {
				throw CqlNotImplementedException(CQL_CODEINFO,
					"unsupported message type:", static_cast<std::size_t>(type));
			}
		}
	}
}

