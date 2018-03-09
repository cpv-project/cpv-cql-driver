#include "./SupportedMessage.hpp"

namespace cql {
	/** For Object */
	void SupportedMessage::reset(MessageHeader&& header) {
		ResponseMessageBase::reset(std::move(header));
		options_.reset();
	}

	/** Get description of this message */
	seastar::sstring SupportedMessage::toString() const {
		return "SupportedMessage()";
	}

	/** Decode message body from binary data */
	void SupportedMessage::decodeBody(
		const ConnectionInfo&, seastar::temporary_buffer<char>&& buffer) {
		// The body of a SUPPORTED message is a [string multimap]
		// This multimap gives for each of the supported STARTUP options, the list of supported values
		const char* ptr = buffer.begin();
		const char* end = buffer.end();
		options_.decode(ptr, end);
	}

	/** Constructor */
	SupportedMessage::SupportedMessage() :
		options_() { }
}

