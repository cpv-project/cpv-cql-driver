#include "CqlAuthenticateMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlAuthenticateMessage::reset(CqlMessageHeader&& header) {
		CqlResponseMessageBase::reset(std::move(header));
		authenticatorClass_.reset();
	}

	/** Get description of this message */
	seastar::sstring CqlAuthenticateMessage::toString() const {
		return joinString("",
			"CqlAuthenticateMessage(authenticatorClass: ", authenticatorClass_.get(), ")");
	}

	/** Decode message body from binary data */
	void CqlAuthenticateMessage::decodeBody(const CqlConnectionInfo&, const char*& ptr, const char* end) {
		// The body consists of a single [string] indicating the full class name of
		// the IAuthenticator to use
		authenticatorClass_.decode(ptr, end);
	}

	/** Constructor */
	CqlAuthenticateMessage::CqlAuthenticateMessage() :
		authenticatorClass_() { }
}

