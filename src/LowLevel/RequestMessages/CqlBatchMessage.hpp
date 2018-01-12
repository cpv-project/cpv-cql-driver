#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Allows executing a list of queries (prepared or not) as a batch
	 * The server will respond with a RESULT message.
	 */
	class CqlBatchMessage : public CqlRequestMessageBase {
	public:
	};
}

