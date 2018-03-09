#include "./AuthResponseMessage.hpp"

namespace cql {
	/** For Object */
	void AuthResponseMessage::reset(MessageHeader&& header) {
		RequestMessageBase::reset(std::move(header));
		token_.reset();
	}

	/** Get description of this message */
	seastar::sstring AuthResponseMessage::toString() const {
		return "AuthResponseMessage()";
	}

	/** Encode message body to binary data */
	void AuthResponseMessage::encodeBody(const ConnectionInfo&, seastar::sstring& data) const {
		// the body of this message is a single [bytes] token.
		// the detals of what this token contains (and when it can be null/empty, if ever)
		// depends on the actual authenticator used.
		token_.encode(data);
	}

	/** Constructor */
	AuthResponseMessage::AuthResponseMessage() :
		token_() { }
}

