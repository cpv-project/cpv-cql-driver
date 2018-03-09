#pragma once
#include <CQLDriver/Common/Utility/Object.hpp>
#include "./RequestMessageBase.hpp"

namespace cql {
	class AuthResponseMessage;
	class BatchMessage;
	class ExecuteMessage;
	class OptionsMessage;
	class PrepareMessage;
	class QueryMessage;
	class RegisterMessage;
	class StartupMessage;

	namespace RequestMessageFactory {
		/** Creates a new request message instance with header set */
		template <class T> Object<T> makeRequestMessage() = delete;
		template <> Object<AuthResponseMessage> makeRequestMessage();
		template <> Object<BatchMessage> makeRequestMessage();
		template <> Object<ExecuteMessage> makeRequestMessage();
		template <> Object<OptionsMessage> makeRequestMessage();
		template <> Object<PrepareMessage> makeRequestMessage();
		template <> Object<QueryMessage> makeRequestMessage();
		template <> Object<RegisterMessage> makeRequestMessage();
		template <> Object<StartupMessage> makeRequestMessage();
	}
}

