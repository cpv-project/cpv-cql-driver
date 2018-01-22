#include "CqlAuthResponseMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlAuthResponseMessage::reset(CqlMessageHeader&& header) {
		CqlRequestMessageBase::reset(std::move(header));
		token_.reset();
	}

	/** Get description of this message */
	seastar::sstring CqlAuthResponseMessage::toString() const {
		return "CqlAuthResponseMessage()";
	}

	/** Encode message body to binary data */
	void CqlAuthResponseMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring& data) const {
		// the body of this message is a single [bytes] token.
		// the detals of what this token contains (and when it can be null/empty, if ever)
		// depends on the actual authenticator used.
		token_.encode(data);
	}

	/** Constructor */
	CqlAuthResponseMessage::CqlAuthResponseMessage() :
		token_() { }
}

