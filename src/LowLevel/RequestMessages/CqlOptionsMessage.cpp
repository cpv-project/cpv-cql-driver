#include "CqlOptionsMessage.hpp"

namespace cql {
	/** Get description of this message */
	seastar::sstring CqlOptionsMessage::toString() const {
		return "CqlOptionsMessage()";
	}

	/** Encode message body to binary data */
	void CqlOptionsMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring&) const {
		// The body of an OPTIONS message should be empty
	}
}

