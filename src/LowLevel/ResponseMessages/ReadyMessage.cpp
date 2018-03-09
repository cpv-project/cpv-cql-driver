#include "./ReadyMessage.hpp"

namespace cql {
	/** Get description of this message */
	seastar::sstring ReadyMessage::toString() const {
		return "ReadyMessage()";
	}

	/** Decode message body from binary data */
	void ReadyMessage::decodeBody(
		const ConnectionInfo&, seastar::temporary_buffer<char>&&) {
		// The body of a READY message is empty
	}
}

