#include "CqlQueryMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlQueryMessage::reset(CqlMessageHeader&& header) {
		CqlRequestMessageBase::reset(std::move(header));
		query_.set(CqlProtocolLongStringState::Normal);
		queryParameters_.setConsistency(CqlConsistencyLevel::Any);
		queryParameters_.resetFlags();
	}

	/** Encode message body to binary data */
	void CqlQueryMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring& data) const {
		// The body of the message must be: <query><query_parameters>
		// where <query> is a [long string] represeting the query, and <query_parameters> must be:
		// (check comments on CqlProtocolQueryParameters)
		query_.encode(data);
		queryParameters_.encode(data);
	}
}

