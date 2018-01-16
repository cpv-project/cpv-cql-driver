#include "CqlExecuteMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlExecuteMessage::reset(CqlMessageHeader&& header) {
		CqlRequestMessageBase::reset(std::move(header));
		prepareQueryId_.set(CqlProtocolShortBytesState::Normal);
		resultMetadataId_.set(CqlProtocolShortBytesState::Normal);
		queryParameters_.setConsistency(CqlConsistencyLevel::Any);
		queryParameters_.resetFlags();
	}

	/** Encode message body to binary data */
	void CqlExecuteMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring& data) const {
		prepareQueryId_.encode(data);
		resultMetadataId_.encode(data);
		queryParameters_.encode(data);
	}
}

