#pragma once
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Prepare a query for later execution (through EXECUTE)
	 * The server will respond with a RESULT message with a "prepared" kind.
	 */
	class CqlPrepareMessage : public CqlRequestMessageBase {
	public:
		using CqlRequestMessageBase::freeResources;
		using CqlRequestMessageBase::reset;

		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;
	};
}

