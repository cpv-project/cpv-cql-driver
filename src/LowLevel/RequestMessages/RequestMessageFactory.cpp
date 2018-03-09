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
		template <> Object<AuthResponseMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::AuthResponse, MessageDirection::Request);
			return makeObject<AuthResponseMessage>(std::move(header));
		}

		template <> Object<BatchMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Batch, MessageDirection::Request);
			return makeObject<BatchMessage>(std::move(header));
		}

		template <> Object<ExecuteMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Execute, MessageDirection::Request);
			return makeObject<ExecuteMessage>(std::move(header));
		}

		template <> Object<OptionsMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Options, MessageDirection::Request);
			return makeObject<OptionsMessage>(std::move(header));
		}

		template <> Object<PrepareMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Prepare, MessageDirection::Request);
			return makeObject<PrepareMessage>(std::move(header));
		}

		template <> Object<QueryMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Query, MessageDirection::Request);
			return makeObject<QueryMessage>(std::move(header));
		}

		template <> Object<RegisterMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Register, MessageDirection::Request);
			return makeObject<RegisterMessage>(std::move(header));
		}

		template <> Object<StartupMessage> makeRequestMessage() {
			MessageHeader header;
			header.setOpCode(MessageType::Startup, MessageDirection::Request);
			return makeObject<StartupMessage>(std::move(header));
		}
	}
}

