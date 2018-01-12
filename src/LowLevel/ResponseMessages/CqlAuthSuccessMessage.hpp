#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/** Indicates the success of the authentication phase */
	class CqlAuthSuccessMessage : public CqlResponseMessageBase {
	public:
		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const seastar::temporary_buffer<char>& data) override;
	};
}

