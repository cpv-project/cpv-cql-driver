#include "CqlReadyMessage.hpp"

namespace cql {
	/** Decode message body from binary data */
	void CqlReadyMessage::decodeBody(const CqlConnectionInfo&, const char*&, const char*) {
		// The body of a READY message is empty
	}
}

