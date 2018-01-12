#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/**
	 * Indicates which startup options are supported by the server
	 * This message comes as a response to an OPTIONS message.
	 */
	class CqlSupportedMessage : public CqlResponseMessageBase {
	};
}

