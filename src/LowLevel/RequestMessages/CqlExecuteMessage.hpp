#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Executes a prepared query
	 * The server will respond a RESULT message, the content of which depends on the query.
	 */
	class CqlExecuteMessage : public CqlRequestMessageBase {
	public:
		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;
	};
}

