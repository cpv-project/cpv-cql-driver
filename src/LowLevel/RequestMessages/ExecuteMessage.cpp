#include "./ExecuteMessage.hpp"

namespace cql {
	/** The storage of ExecuteMessage */
	template <>
	thread_local ReusableStorageType<ExecuteMessage>
		ReusableStorageInstance<ExecuteMessage>;

	/** For Reusable */
	void ExecuteMessage::reset(MessageHeader&& header) {
		RequestMessageBase::reset(std::move(header));
		preparedQueryId_.reset();
		queryParameters_.reset();
	}

	/** Get description of this message */
	std::string ExecuteMessage::toString() const {
		return "ExecuteMessage()";
	}

	/** Encode message body to binary data */
	void ExecuteMessage::encodeBody(const ConnectionInfo&, std::string& data) const {
		// The body of the message must be: <id><query_parameters>
		preparedQueryId_.encode(data);
		queryParameters_.encode(data);
	}

	/** Constructor */
	ExecuteMessage::ExecuteMessage() :
		preparedQueryId_(),
		queryParameters_() { }
}

