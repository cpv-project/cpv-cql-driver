#include "CqlSupportedMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlSupportedMessage::reset(CqlMessageHeader&& header) {
		CqlResponseMessageBase::reset(std::move(header));
		options_.reset();
	}

	/** Get description of this message */
	seastar::sstring CqlSupportedMessage::toString() const {
		return "CqlSupportedMessage()";
	}

	/** Decode message body from binary data */
	void CqlSupportedMessage::decodeBody(const CqlConnectionInfo&, const char*& ptr, const char* end) {
		// The body of a SUPPORTED message is a [string multimap]
		// This multimap gives for each of the supported STARTUP options, the list of supported values
		options_.decode(ptr, end);
	}

	/** Constructor */
	CqlSupportedMessage::CqlSupportedMessage() :
		options_() { }
}

