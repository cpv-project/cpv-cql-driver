#include "CqlReadyMessage.hpp"

namespace cql {
	/** Get description of this message */
	seastar::sstring CqlReadyMessage::toString() const {
		return "CqlReadyMessage()";
	}

	/** Decode message body from binary data */
	void CqlReadyMessage::decodeBody(const CqlConnectionInfo&, const char*&, const char*) {
		// The body of a READY message is empty
	}
}

