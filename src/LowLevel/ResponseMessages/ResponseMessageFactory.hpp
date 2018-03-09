#pragma once
#include <CQLDriver/Common/Utility/Object.hpp>
#include "../MessageHeader.hpp"
#include "./ResponseMessageBase.hpp"

namespace cql {
	namespace ResponseMessageFactory {
		/** Creates a new response message instance with header set */
		Object<ResponseMessageBase> makeResponseMessage(MessageHeader&& header);
	}
}

