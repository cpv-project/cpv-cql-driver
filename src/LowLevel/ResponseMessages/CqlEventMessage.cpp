#include "CqlEventMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlEventMessage::reset(CqlMessageHeader&& header) {
		CqlResponseMessageBase::reset(std::move(header));
		type_.reset();
		data_.resize(0);
	}

	/** Get description of this message */
	seastar::sstring CqlEventMessage::toString() const {
		return "CqlEventMessage()";
	}

	/** Decode message body from binary data */
	void CqlEventMessage::decodeBody(
		const CqlConnectionInfo&, seastar::temporary_buffer<char>&& buffer) {
		// the body of an EVENT message will start with a [string] represeting the event type,
		// the reset of the message depends on the event type.
		const char* ptr = buffer.begin();
		const char* end = buffer.end();
		type_.decode(ptr, end);
		data_.resize(0);
		data_.append(ptr, end - ptr);
	}

	/** Constructor */
	CqlEventMessage::CqlEventMessage() :
		type_(), data_() { }
}

