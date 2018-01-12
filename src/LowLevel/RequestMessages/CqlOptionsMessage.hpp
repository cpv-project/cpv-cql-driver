#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Asks the server to return which STARTUP options are supported
	 * The server will respond with a SUPPORTED message.
	 */
	class CqlOptionsMessage : public CqlRequestMessageBase {
	public:
	};
}

