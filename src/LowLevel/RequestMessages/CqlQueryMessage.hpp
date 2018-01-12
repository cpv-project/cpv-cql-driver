#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Performs a CQL query
	 * The server will respond a RESULT message, the content of which depends on the query.
	 */
	class CqlQueryMessage : public CqlRequestMessageBase {
	public:
		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;
	};
}

