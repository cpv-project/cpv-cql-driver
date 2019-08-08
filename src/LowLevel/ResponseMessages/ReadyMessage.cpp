#include "./ReadyMessage.hpp"

namespace cql {
	/** The storage of ReadyMessage */
	template <>
	thread_local ReusableStorageType<ReadyMessage>
		ReusableStorageInstance<ReadyMessage>;

	/** Get description of this message */
	std::string ReadyMessage::toString() const {
		return "ReadyMessage()";
	}

	/** Decode message body from binary data */
	void ReadyMessage::decodeBody(
		const ConnectionInfo&, seastar::temporary_buffer<char>&&) {
		// The body of a READY message is empty
	}
}

