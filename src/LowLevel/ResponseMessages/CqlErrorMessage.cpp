#include "CqlErrorMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlErrorMessage::reset(CqlMessageHeader&& header) {
		CqlResponseMessageBase::reset(std::move(header));
		errorCode_.reset();
		errorMessage_.reset();
		extraContents_.resize(0);
	}

	/** Decode message body from binary data */
	void CqlErrorMessage::decodeBody(const CqlConnectionInfo&, const char*& ptr, const char* end) {
		errorCode_.decode(ptr, end);
		errorMessage_.decode(ptr, end);
		extraContents_.resize(0);
		extraContents_.append(ptr, end - ptr);
	}

	/** Constructor */
	CqlErrorMessage::CqlErrorMessage() :
		errorCode_(),
		errorMessage_(),
		extraContents_() { }
}

