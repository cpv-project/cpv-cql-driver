#include "./AuthSuccessMessage.hpp"

namespace cql {
	/** For Object */
	void AuthSuccessMessage::reset(MessageHeader&& header) {
		ResponseMessageBase::reset(std::move(header));
		token_.reset();
	}

	/** Get description of this message */
	seastar::sstring AuthSuccessMessage::toString() const {
		return "AuthSuccessMessage()";
	}

	/** Decode message body from binary data */
	void AuthSuccessMessage::decodeBody(
		const ConnectionInfo&, seastar::temporary_buffer<char>&& buffer) {
		// the body of this message is a single [byte]
		const char* ptr = buffer.begin();
		const char* end = buffer.end();
		token_.decode(ptr, end);
	}

	/** Constructor */
	AuthSuccessMessage::AuthSuccessMessage() :
		token_() { }
}

