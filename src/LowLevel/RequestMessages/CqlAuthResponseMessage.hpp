#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Answers a server authentication challenge
	 * The response to a AUTH_RESPONSE is either a follow-up AUTH_CHALLENGE message,
	 * an AUTH_SUCCESS message or an ERROR message.
	 */
	class CqlAuthResponseMessage : public CqlRequestMessageBase {
	public:
	};
}

