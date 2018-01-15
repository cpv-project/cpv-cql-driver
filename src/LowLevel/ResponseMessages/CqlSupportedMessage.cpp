#include "CqlSupportedMessage.hpp"

namespace cql {
	/** Decode message body from binary data */
	void CqlSupportedMessage::decodeBody(const CqlConnectionInfo&, const seastar::temporary_buffer<char>&) {
		// TODO
	}
}

