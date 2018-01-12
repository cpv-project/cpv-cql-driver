#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/** The result to a query (QUERY, PREPARE, EXECUTE or BATCH message */
	class CqlResultMessage : public CqlResponseMessageBase {
	};
}

