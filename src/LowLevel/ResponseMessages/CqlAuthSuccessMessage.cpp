#include "CqlAuthSuccessMessage.hpp"

namespace cql {
	/** Decode message body from binary data */
	void CqlAuthSuccessMessage::decodeBody(const CqlConnectionInfo&, const seastar::temporary_buffer<char>&) {
		// TODO
	}
}

