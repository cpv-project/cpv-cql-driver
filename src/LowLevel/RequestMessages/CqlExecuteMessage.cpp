#include "CqlExecuteMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlExecuteMessage::reset(CqlMessageHeader&& header) {
		CqlRequestMessageBase::reset(std::move(header));
		preparedQueryId_.reset();
		resultMetadataId_.reset();
		queryParameters_.reset();
	}

	/** Encode message body to binary data */
	void CqlExecuteMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring& data) const {
		preparedQueryId_.encode(data);
		resultMetadataId_.encode(data);
		queryParameters_.encode(data);
	}
}

