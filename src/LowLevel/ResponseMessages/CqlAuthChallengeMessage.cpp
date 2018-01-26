#include "CqlAuthChallengeMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlAuthChallengeMessage::reset(CqlMessageHeader&& header) {
		CqlResponseMessageBase::reset(std::move(header));
		token_.reset();
	}

	/** Get description of this message */
	seastar::sstring CqlAuthChallengeMessage::toString() const {
		return "CqlAuthChallengeMessage()";
	}

	/** Decode message body from binary data */
	void CqlAuthChallengeMessage::decodeBody(
		const CqlConnectionInfo&, seastar::temporary_buffer<char>&& buffer) {
		// the body of this message is a single [bytes] token
		const char* ptr = buffer.begin();
		const char* end = buffer.end();
		token_.decode(ptr, end);
	}

	/** Constructor */
	CqlAuthChallengeMessage::CqlAuthChallengeMessage() :
		token_() { }
}

