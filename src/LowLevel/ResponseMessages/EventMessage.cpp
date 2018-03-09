#include "./EventMessage.hpp"

namespace cql {
	/** For Object */
	void EventMessage::reset(MessageHeader&& header) {
		ResponseMessageBase::reset(std::move(header));
		type_.reset();
		data_.resize(0);
	}

	/** Get description of this message */
	seastar::sstring EventMessage::toString() const {
		return "EventMessage()";
	}

	/** Decode message body from binary data */
	void EventMessage::decodeBody(
		const ConnectionInfo&, seastar::temporary_buffer<char>&& buffer) {
		// the body of an EVENT message will start with a [string] represeting the event type,
		// the reset of the message depends on the event type.
		const char* ptr = buffer.begin();
		const char* end = buffer.end();
		type_.decode(ptr, end);
		data_.resize(0);
		data_.append(ptr, end - ptr);
	}

	/** Constructor */
	EventMessage::EventMessage() :
		type_(), data_() { }
}

