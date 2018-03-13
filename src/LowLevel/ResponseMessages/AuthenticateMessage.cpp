#include "./AuthenticateMessage.hpp"

namespace cql {
	/** For Object */
	void AuthenticateMessage::reset(MessageHeader&& header) {
		ResponseMessageBase::reset(std::move(header));
		authenticatorClass_.reset();
	}

	/** Get description of this message */
	std::string AuthenticateMessage::toString() const {
		return joinString("",
			"AuthenticateMessage(authenticatorClass: ", authenticatorClass_.get(), ")");
	}

	/** Decode message body from binary data */
	void AuthenticateMessage::decodeBody(
		const ConnectionInfo&, seastar::temporary_buffer<char>&& buffer) {
		// The body consists of a single [string] indicating the full class name of
		// the IAuthenticator to use
		const char* ptr = buffer.begin();
		const char* end = buffer.end();
		authenticatorClass_.decode(ptr, end);
	}

	/** Constructor */
	AuthenticateMessage::AuthenticateMessage() :
		authenticatorClass_() { }
}

