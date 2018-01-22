#pragma once
#include "../ProtocolTypes/CqlProtocolLongString.hpp"
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Prepare a query for later execution (through EXECUTE)
	 * The server will respond with a RESULT message with a "prepared" kind.
	 */
	class CqlPrepareMessage : public CqlRequestMessageBase {
	public:
		using CqlRequestMessageBase::freeResources;

		/** For CqlObject */
		void reset(CqlMessageHeader&& header);

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;

		/** The CQL query string */
		const CqlProtocolLongString& getQuery() const& { return query_; }
		CqlProtocolLongString& getQuery() & { return query_; }

		/** Constructor */
		CqlPrepareMessage();

	private:
		CqlProtocolLongString query_;
	};
}

