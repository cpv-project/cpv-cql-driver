#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>
#include "./ResponseMessageFactory.hpp"
#include "./ResponseMessageBase.hpp"
#include "./AuthChallengeMessage.hpp"
#include "./AuthenticateMessage.hpp"
#include "./AuthSuccessMessage.hpp"
#include "./ErrorMessage.hpp"
#include "./EventMessage.hpp"
#include "./ReadyMessage.hpp"
#include "./ResultMessage.hpp"
#include "./SupportedMessage.hpp"

namespace cql {
	namespace ResponseMessageFactory {
		/** Creates a new response message instance with header set */
		Reusable<ResponseMessageBase> makeResponseMessage(MessageHeader&& header) {
			// order by frequency
			// RESULT > ERROR > EVENT > READY > SUPPORTED > AUTHENTICATE > AUTH_SUCCESS > AUTH_CHALLENGE
			MessageType type = header.getOpCode();
			if (type == MessageType::Result) {
				return makeReusable<ResultMessage>(std::move(header)).cast<ResponseMessageBase>();
			} else if (type == MessageType::Error) {
				return makeReusable<ErrorMessage>(std::move(header)).cast<ResponseMessageBase>();
			} else if (type == MessageType::Event) {
				return makeReusable<EventMessage>(std::move(header)).cast<ResponseMessageBase>();
			} else if (type == MessageType::Ready) {
				return makeReusable<ReadyMessage>(std::move(header)).cast<ResponseMessageBase>();
			} else if (type == MessageType::Supported) {
				return makeReusable<SupportedMessage>(std::move(header)).cast<ResponseMessageBase>();
			} else if (type == MessageType::Authenticate) {
				return makeReusable<AuthenticateMessage>(std::move(header)).cast<ResponseMessageBase>();
			} else if (type == MessageType::AuthSuccess) {
				return makeReusable<AuthSuccessMessage>(std::move(header)).cast<ResponseMessageBase>();
			} else if (type == MessageType::AuthChallenge) {
				return makeReusable<AuthChallengeMessage>(std::move(header)).cast<ResponseMessageBase>();
			} else {
				throw NotImplementedException(CQL_CODEINFO,
					"unsupported message type:", static_cast<std::size_t>(type));
			}
		}
	}
}

