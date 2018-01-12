#pragma once
#include <CqlDriver/Common/Utility/CqlObject.hpp>
#include "CqlRequestMessageBase.hpp"

namespace cql {
	class CqlAuthResponseMessage;
	class CqlBatchMessage;
	class CqlExecuteMessage;
	class CqlOptionsMessage;
	class CqlPrepareMessage;
	class CqlQueryMessage;
	class CqlRegisterMessage;
	class CqlStartupMessage;

	namespace CqlRequestMessageFactory {
		/** Creates a new request message instance with header set */
		template <class T> CqlObject<T> makeRequestMessage() = delete;
		template <> CqlObject<CqlAuthResponseMessage> makeRequestMessage();
		template <> CqlObject<CqlBatchMessage> makeRequestMessage();
		template <> CqlObject<CqlExecuteMessage> makeRequestMessage();
		template <> CqlObject<CqlOptionsMessage> makeRequestMessage();
		template <> CqlObject<CqlPrepareMessage> makeRequestMessage();
		template <> CqlObject<CqlQueryMessage> makeRequestMessage();
		template <> CqlObject<CqlRegisterMessage> makeRequestMessage();
		template <> CqlObject<CqlStartupMessage> makeRequestMessage();
	}
}

