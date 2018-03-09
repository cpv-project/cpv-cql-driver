#include "./ExecuteMessage.hpp"

namespace cql {
	/** For Object */
	void ExecuteMessage::reset(MessageHeader&& header) {
		RequestMessageBase::reset(std::move(header));
		preparedQueryId_.reset();
		queryParameters_.reset();
	}

	/** Get description of this message */
	seastar::sstring ExecuteMessage::toString() const {
		return "ExecuteMessage()";
	}

	/** Encode message body to binary data */
	void ExecuteMessage::encodeBody(const ConnectionInfo&, seastar::sstring& data) const {
		// The body of the message must be: <id><query_parameters>
		preparedQueryId_.encode(data);
		queryParameters_.encode(data);
	}

	/** Constructor */
	ExecuteMessage::ExecuteMessage() :
		preparedQueryId_(),
		queryParameters_() { }
}

