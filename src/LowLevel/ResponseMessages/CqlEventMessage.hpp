#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/**
	 * An event pushed by the server
	 * A client will only receive events for the types it has REGISTERed to.
	 */
	class CqlEventMessage : public CqlResponseMessageBase {
	};
}

