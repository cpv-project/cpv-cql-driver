#pragma once
#include <CQLDriver/Common/Utility/Reusable.hpp>
#include "../MessageHeader.hpp"
#include "./ResponseMessageBase.hpp"

namespace cql {
	namespace ResponseMessageFactory {
		/** Creates a new response message instance with header set */
		Reusable<ResponseMessageBase> makeResponseMessage(MessageHeader&& header);
	}
}

