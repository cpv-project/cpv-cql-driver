#include "CqlRegisterMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlRegisterMessage::reset(CqlMessageHeader&& header) {
		CqlRequestMessageBase::reset(std::move(header));
		events_.reset();
	}

	/** Encode message body to binary data */
	void CqlRegisterMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring& data) const {
		// The body of the message is a [string list] representing the event types to register for.
		// See section 4.2.6 for the list of valid event types.
		events_.encode(data);
	}

	/** Constructor */
	CqlRegisterMessage::CqlRegisterMessage() :
		events_() { }
}

