#include "CqlExecuteMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlExecuteMessage::reset(CqlMessageHeader&& header) {
		CqlRequestMessageBase::reset(std::move(header));
		preparedQueryId_.reset();
		queryParameters_.reset();
	}

	/** Get description of this message */
	seastar::sstring CqlExecuteMessage::toString() const {
		return "CqlExecuteMessage()";
	}

	/** Encode message body to binary data */
	void CqlExecuteMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring& data) const {
		// The body of the message must be: <id><query_parameters>
		preparedQueryId_.encode(data);
		queryParameters_.encode(data);
	}

	/** Constructor */
	CqlExecuteMessage::CqlExecuteMessage() :
		preparedQueryId_(),
		queryParameters_() { }
}

