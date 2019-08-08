#include "./RequestMessageFactory.hpp"
#include "./AuthResponseMessage.hpp"
#include "./BatchMessage.hpp"
#include "./ExecuteMessage.hpp"
#include "./OptionsMessage.hpp"
#include "./PrepareMessage.hpp"
#include "./QueryMessage.hpp"
#include "./RegisterMessage.hpp"
#include "./StartupMessage.hpp"

namespace cql {
	namespace RequestMessageFactory {
		template <> Reusable<AuthResponseMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::AuthResponse, MessageDirection::Request);
			return makeReusable<AuthResponseMessage>(std::move(header));
		}

		template <> Reusable<BatchMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Batch, MessageDirection::Request);
			return makeReusable<BatchMessage>(std::move(header));
		}

		template <> Reusable<ExecuteMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Execute, MessageDirection::Request);
			return makeReusable<ExecuteMessage>(std::move(header));
		}

		template <> Reusable<OptionsMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Options, MessageDirection::Request);
			return makeReusable<OptionsMessage>(std::move(header));
		}

		template <> Reusable<PrepareMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Prepare, MessageDirection::Request);
			return makeReusable<PrepareMessage>(std::move(header));
		}

		template <> Reusable<QueryMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Query, MessageDirection::Request);
			return makeReusable<QueryMessage>(std::move(header));
		}

		template <> Reusable<RegisterMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Register, MessageDirection::Request);
			return makeReusable<RegisterMessage>(std::move(header));
		}

		template <> Reusable<StartupMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Startup, MessageDirection::Request);
			return makeReusable<StartupMessage>(std::move(header));
		}
	}
}

