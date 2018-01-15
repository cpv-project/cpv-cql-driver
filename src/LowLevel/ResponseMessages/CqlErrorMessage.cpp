#include "CqlErrorMessage.hpp"

namespace cql {
	/** Decode message body from binary data */
	void CqlErrorMessage::decodeBody(const CqlConnectionInfo&, const seastar::temporary_buffer<char>&) {
		// TODO
	}
}

