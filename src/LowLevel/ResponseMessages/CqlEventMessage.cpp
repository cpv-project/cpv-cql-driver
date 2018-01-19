#include "CqlEventMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlEventMessage::reset(CqlMessageHeader&& header) {
		CqlResponseMessageBase::reset(std::move(header));
		type_.reset();
		data_.resize(0);
	}

	/** Decode message body from binary data */
	void CqlEventMessage::decodeBody(const CqlConnectionInfo&, const char*& ptr, const char* end) {
		// the body of an EVENT message will start with a [string] represeting the event type,
		// the reset of the message depends on the event type.
		type_.decode(ptr, end);
		data_.resize(0);
		data_.append(ptr, end - ptr);
		ptr = end;
	}

	/** Constructor */
	CqlEventMessage::CqlEventMessage() :
		type_(), data_() { }
}

