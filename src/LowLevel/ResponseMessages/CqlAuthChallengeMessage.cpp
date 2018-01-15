#include "CqlAuthChallengeMessage.hpp"

namespace cql {
	/** Decode message body from binary data */
	void CqlAuthChallengeMessage::decodeBody(const CqlConnectionInfo&, const seastar::temporary_buffer<char>&) {
		// TODO
	}
}

