#include "./OptionsMessage.hpp"

namespace cql {
	/** Get description of this message */
	seastar::sstring OptionsMessage::toString() const {
		return "OptionsMessage()";
	}

	/** Encode message body to binary data */
	void OptionsMessage::encodeBody(const ConnectionInfo&, seastar::sstring&) const {
		// The body of an OPTIONS message should be empty
	}
}

