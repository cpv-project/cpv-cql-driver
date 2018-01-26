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
	void CqlSupportedMessage::decodeBody(
		const CqlConnectionInfo&, seastar::temporary_buffer<char>&& buffer) {
		// The body of a SUPPORTED message is a [string multimap]
		// This multimap gives for each of the supported STARTUP options, the list of supported values
		const char* ptr = buffer.begin();
		const char* end = buffer.end();
		options_.decode(ptr, end);
	}

	/** Constructor */
	CqlSupportedMessage::CqlSupportedMessage() :
		options_() { }
}

