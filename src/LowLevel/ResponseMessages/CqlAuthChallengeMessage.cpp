#include "CqlAuthChallengeMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlAuthChallengeMessage::reset(CqlMessageHeader&& header) {
		CqlResponseMessageBase::reset(std::move(header));
		token_.reset();
	}

	/** Decode message body from binary data */
	void CqlAuthChallengeMessage::decodeBody(const CqlConnectionInfo&, const char*& ptr, const char* end) {
		// the body of this message is a single [bytes] token
		token_.decode(ptr, end);
	}

	/** Constructor */
	CqlAuthChallengeMessage::CqlAuthChallengeMessage() :
		token_() { }
}

