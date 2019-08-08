#include "./PrepareMessage.hpp"

namespace cql {
	/** The storage of PrepareMessage */
	template <>
	thread_local ReusableStorageType<PrepareMessage>
		ReusableStorageInstance<PrepareMessage>;

	/** For Reusable */
	void PrepareMessage::reset(MessageHeader&& header) {
		RequestMessageBase::reset(std::move(header));
		query_.reset();
	}

	/** Get description of this message */
	std::string PrepareMessage::toString() const {
		return joinString("", "PrepareMessage(query: ", query_.get(), ")");
	}

	/** Encode message body to binary data */
	void PrepareMessage::encodeBody(const ConnectionInfo&, std::string& data) const {
		// The body of the message must be: <query>
		// where <query> is a [long string] representing the CQL query.
		query_.encode(data);
	}

	/** Constructor */
	PrepareMessage::PrepareMessage() :
		query_() { }
}

