#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Asks the server to return which STARTUP options are supported
	 * The server will respond with a SUPPORTED message.
	 */
	class CqlOptionsMessage : public CqlRequestMessageBase {
	public:
		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;
	};
}

