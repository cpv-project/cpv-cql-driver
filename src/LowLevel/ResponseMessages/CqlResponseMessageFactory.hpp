#pragma once
#include <CqlDriver/Common/Utility/CqlObject.hpp>
#include "../CqlMessageHeader.hpp"
#include "CqlResponseMessageBase.hpp"

namespace cql {
	namespace CqlResponseMessageFactory {
		/** Creates a new response message instance with header set */
		CqlObject<CqlResponseMessageBase> makeResponseMessage(CqlMessageHeader&& header);
	}
}

