#include "CqlPrepareMessage.hpp"

namespace cql {
	/** For CqlObject */
	void CqlPrepareMessage::reset(CqlMessageHeader&& header) {
		CqlRequestMessageBase::reset(std::move(header));
		query_.reset();
	}

	/** Encode message body to binary data */
	void CqlPrepareMessage::encodeBody(const CqlConnectionInfo&, seastar::sstring& data) const {
		// The body of the message must be: <query>
		// where <query> is a [long string] representing the CQL query.
		query_.encode(data);
	}

	/** Constructor */
	CqlPrepareMessage::CqlPrepareMessage() :
		query_() { }
}

