#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/**
	 * Indicates which startup options are supported by the server
	 * This message comes as a response to an OPTIONS message.
	 */
	class CqlSupportedMessage : public CqlResponseMessageBase {
	public:
		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const seastar::temporary_buffer<char>& data) override;
	};
}

