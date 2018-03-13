#include "./AuthChallengeMessage.hpp"

namespace cql {
	/** For Object */
	void AuthChallengeMessage::reset(MessageHeader&& header) {
		ResponseMessageBase::reset(std::move(header));
		token_.reset();
	}

	/** Get description of this message */
	std::string AuthChallengeMessage::toString() const {
		return "AuthChallengeMessage()";
	}

	/** Decode message body from binary data */
	void AuthChallengeMessage::decodeBody(
		const ConnectionInfo&, seastar::temporary_buffer<char>&& buffer) {
		// the body of this message is a single [bytes] token
		const char* ptr = buffer.begin();
		const char* end = buffer.end();
		token_.decode(ptr, end);
	}

	/** Constructor */
	AuthChallengeMessage::AuthChallengeMessage() :
		token_() { }
}

