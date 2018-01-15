#include "CqlOptionsMessage.hpp"

namespace cql {
	/** Encode message body to binary data */
	void CqlOptionsMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring&) const {
		// The body of an OPTIONS message should be empty
	}
}

