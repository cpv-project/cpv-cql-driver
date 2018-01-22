#include "CqlAuthSuccessMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlAuthSuccessMessage::reset(CqlMessageHeader&& header) {
		CqlResponseMessageBase::reset(std::move(header));
		token_.reset();
	}

	/** Get description of this message */
	seastar::sstring CqlAuthSuccessMessage::toString() const {
		return "AuthSuccessMessage()";
	}

	/** Decode message body from binary data */
	void CqlAuthSuccessMessage::decodeBody(const CqlConnectionInfo&, const char*& ptr, const char* end) {
		// the body of this message is a single [byte]
		token_.decode(ptr, end);
	}

	/** Constructor */
	CqlAuthSuccessMessage::CqlAuthSuccessMessage() :
		token_() { }
}

