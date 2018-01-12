#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/**
	 * Indicates that the server requires authentication, and which authentication mechaism to use
	 * This message will be sent following a STARTUP message if authentication is required
	 * and must be answered by a AUTH_RESPONSE message from the client.
	 */
	class CqlAuthenticateMessage : public CqlResponseMessageBase {
	};
}

