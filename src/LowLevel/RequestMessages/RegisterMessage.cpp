#include "./RegisterMessage.hpp"

namespace cql {
	/** The storage of RegisterMessage */
	template <>
	thread_local ReusableStorageType<RegisterMessage>
		ReusableStorageInstance<RegisterMessage>;

	/** For Reusable */
	void RegisterMessage::reset(MessageHeader&& header) {
		RequestMessageBase::reset(std::move(header));
		events_.reset();
	}

	/** Get description of this message */
	std::string RegisterMessage::toString() const {
		return "RegisterMessage()";
	}

	/** Encode message body to binary data */
	void RegisterMessage::encodeBody(const ConnectionInfo&, std::string& data) const {
		// The body of the message is a [string list] representing the event types to register for.
		// See section 4.2.6 for the list of valid event types.
		events_.encode(data);
	}

	/** Constructor */
	RegisterMessage::RegisterMessage() :
		events_() { }
}

