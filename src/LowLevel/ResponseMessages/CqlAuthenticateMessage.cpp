#include "CqlAuthenticateMessage.hpp"

namespace cql {
	/** Decode message body from binary data */
	void CqlAuthenticateMessage::decodeBody(const CqlConnectionInfo&, const seastar::temporary_buffer<char>&) {
		// TODO
	}
}

