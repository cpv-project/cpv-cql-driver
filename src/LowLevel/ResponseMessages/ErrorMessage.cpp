#include "./ErrorMessage.hpp"

namespace cql {
	/** The storage of ErrorMessage */
	template <>
	thread_local ReusableStorageType<ErrorMessage>
		ReusableStorageInstance<ErrorMessage>;

	/** For Reusable */
	void ErrorMessage::reset(MessageHeader&& header) {
		ResponseMessageBase::reset(std::move(header));
		errorCode_.reset();
		errorMessage_.reset();
		extraContents_.resize(0);
	}

	/** Get description of this message */
	std::string ErrorMessage::toString() const {
		return joinString("",
			"ErrorMessage(errorCode: ", getErrorCode(),
			", errorCodeInt: ", errorCode_.get(),
			", errorMessage: ", errorMessage_.get(), ")");
	}

	/** Decode message body from binary data */
	void ErrorMessage::decodeBody(
		const ConnectionInfo&, seastar::temporary_buffer<char>&& buffer) {
		// the body of the message will be an [int] error code followed by a [string] error message
		// then, depending on the exception, more content may follow
		const char* ptr = buffer.begin();
		const char* end = buffer.end();
		errorCode_.decode(ptr, end);
		errorMessage_.decode(ptr, end);
		extraContents_.resize(0);
		extraContents_.append(ptr, end - ptr);
	}

	/** Constructor */
	ErrorMessage::ErrorMessage() :
		errorCode_(),
		errorMessage_(),
		extraContents_() { }
}

