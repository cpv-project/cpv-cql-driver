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
		query_.encode(data);
		queryParameters_.encode(data);
	}
}

