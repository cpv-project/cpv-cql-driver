#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/**
	 * A server authentication challenge
	 * Clients are expected to answer the server challenge with an AUTH_RESPONSE message.
	 */
	class CqlAuthChallengeMessage : public CqlResponseMessageBase {
	};
}

