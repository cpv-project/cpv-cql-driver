#include "CqlEventMessage.hpp"

namespace cql {
	/** Decode message body from binary data */
	void CqlEventMessage::decodeBody(const CqlConnectionInfo&, const seastar::temporary_buffer<char>&) {
		// TODO
	}
}

