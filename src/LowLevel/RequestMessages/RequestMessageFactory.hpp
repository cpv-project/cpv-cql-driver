#pragma once
#include <CQLDriver/Common/Utility/Reusable.hpp>
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
		template <class T> Reusable<T> makeRequestMessage() = delete;
		template <> Reusable<AuthResponseMessage> makeRequestMessage();
		template <> Reusable<BatchMessage> makeRequestMessage();
		template <> Reusable<ExecuteMessage> makeRequestMessage();
		template <> Reusable<OptionsMessage> makeRequestMessage();
		template <> Reusable<PrepareMessage> makeRequestMessage();
		template <> Reusable<QueryMessage> makeRequestMessage();
		template <> Reusable<RegisterMessage> makeRequestMessage();
		template <> Reusable<StartupMessage> makeRequestMessage();
	}
}

