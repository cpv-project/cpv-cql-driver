#include "CqlPrepareMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlPrepareMessage::reset(CqlMessageHeader&& header) {
		CqlRequestMessageBase::reset(std::move(header));
		query_.set(CqlProtocolLongStringState::Normal);
		prepareParameters_.resetFlags();
	}

	/** Encode message body to binary data */
	void CqlPrepareMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring& data) const {
		// The body of the message must be: <query><flags>[<keyspace>]
		// where <query> is a [long string] representing the CQL query.
		// The remain parts are in CqlProtocolPrepareParameters.
		query_.encode(data);
		prepareParameters_.encode(data);
	}
}

