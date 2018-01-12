#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/** Indicates an error processing a request */
	class CqlErrorMessage : public CqlResponseMessageBase {
	};
}

