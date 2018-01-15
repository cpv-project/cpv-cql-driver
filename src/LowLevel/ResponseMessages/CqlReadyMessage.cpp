#include "CqlReadyMessage.hpp"

namespace cql {
	/** Decode message body from binary data */
	void CqlReadyMessage::decodeBody(const CqlConnectionInfo&, const seastar::temporary_buffer<char>&) {
		// TODO
	}
}

