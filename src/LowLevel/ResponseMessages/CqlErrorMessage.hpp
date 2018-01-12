#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/** Indicates an error processing a request */
	class CqlErrorMessage : public CqlResponseMessageBase {
	public:
		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const seastar::temporary_buffer<char>& data) override;
	};
}

