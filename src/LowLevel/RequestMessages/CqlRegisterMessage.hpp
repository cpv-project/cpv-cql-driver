#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Register this connection to receive some types of events
	 * The response to a REGISTER message will be a READY message.
	 */
	class CqlRegisterMessage : public CqlRequestMessageBase {
	public:
	};
}

