#include "./PrepareMessage.hpp"

namespace cql {
	/** For Object */
	void PrepareMessage::reset(MessageHeader&& header) {
		RequestMessageBase::reset(std::move(header));
		query_.reset();
	}

	/** Get description of this message */
	seastar::sstring PrepareMessage::toString() const {
		return joinString("", "PrepareMessage(query: ", query_.get(), ")");
	}

	/** Encode message body to binary data */
	void PrepareMessage::encodeBody(const ConnectionInfo&, seastar::sstring& data) const {
		// The body of the message must be: <query>
		// where <query> is a [long string] representing the CQL query.
		query_.encode(data);
	}

	/** Constructor */
	PrepareMessage::PrepareMessage() :
		query_() { }
}

