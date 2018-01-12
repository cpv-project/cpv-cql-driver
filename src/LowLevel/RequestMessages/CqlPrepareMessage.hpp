#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Prepare a query for later execution (through EXECUTE)
	 * The server will respond with a RESULT message with a "prepared" kind.
	 */
	class CqlPrepareMessage : public CqlRequestMessageBase {
	public:
	};
}

