#include "CqlRequestMessageFactory.hpp"
#include "CqlAuthResponseMessage.hpp"
#include "CqlBatchMessage.hpp"
#include "CqlExecuteMessage.hpp"
#include "CqlOptionsMessage.hpp"
#include "CqlPrepareMessage.hpp"
#include "CqlQueryMessage.hpp"
#include "CqlRegisterMessage.hpp"
#include "CqlStartupMessage.hpp"

namespace cql {
	namespace CqlRequestMessageFactory {
		template <> CqlObject<CqlAuthResponseMessage> makeRequestMessage() {
			CqlMessageHeader header;
			header.setOpCode(CqlMessageType::AuthResponse, CqlMessageDirection::Request);
			return makeObject<CqlAuthResponseMessage>(std::move(header));
		}

		template <> CqlObject<CqlBatchMessage> makeRequestMessage() {
			CqlMessageHeader header;
			header.setOpCode(CqlMessageType::Batch, CqlMessageDirection::Request);
			return makeObject<CqlBatchMessage>(std::move(header));
		}

		template <> CqlObject<CqlExecuteMessage> makeRequestMessage() {
			CqlMessageHeader header;
			header.setOpCode(CqlMessageType::Execute, CqlMessageDirection::Request);
			return makeObject<CqlExecuteMessage>(std::move(header));
		}

		template <> CqlObject<CqlOptionsMessage> makeRequestMessage() {
			CqlMessageHeader header;
			header.setOpCode(CqlMessageType::Options, CqlMessageDirection::Request);
			return makeObject<CqlOptionsMessage>(std::move(header));
		}

		template <> CqlObject<CqlPrepareMessage> makeRequestMessage() {
			CqlMessageHeader header;
			header.setOpCode(CqlMessageType::Prepare, CqlMessageDirection::Request);
			return makeObject<CqlPrepareMessage>(std::move(header));
		}

		template <> CqlObject<CqlQueryMessage> makeRequestMessage() {
			CqlMessageHeader header;
			header.setOpCode(CqlMessageType::Query, CqlMessageDirection::Request);
			return makeObject<CqlQueryMessage>(std::move(header));
		}

		template <> CqlObject<CqlRegisterMessage> makeRequestMessage() {
			CqlMessageHeader header;
			header.setOpCode(CqlMessageType::Register, CqlMessageDirection::Request);
			return makeObject<CqlRegisterMessage>(std::move(header));
		}

		template <> CqlObject<CqlStartupMessage> makeRequestMessage() {
			CqlMessageHeader header;
			header.setOpCode(CqlMessageType::Startup, CqlMessageDirection::Request);
			return makeObject<CqlStartupMessage>(std::move(header));
		}
	}
}

