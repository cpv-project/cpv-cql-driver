#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Allows executing a list of queries (prepared or not) as a batch
	 * The server will respond with a RESULT message.
	 */
	class CqlBatchMessage : public CqlRequestMessageBase {
	public:
		using CqlRequestMessageBase::freeResources;
		using CqlRequestMessageBase::reset;

		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;
	};
}

