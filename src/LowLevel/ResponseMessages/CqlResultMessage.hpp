#pragma once
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/** The result to a query (QUERY, PREPARE, EXECUTE or BATCH message */
	class CqlResultMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;
		using CqlResponseMessageBase::reset;

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const seastar::temporary_buffer<char>& data) override;
	};
}

