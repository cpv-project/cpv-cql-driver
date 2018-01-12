#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Initialize the connection
	 * The server will respond by either a READY message or an AUTHENTICATE message.
	 */
	class CqlStartupMessage : public CqlRequestMessageBase {
	public:
	};
}

