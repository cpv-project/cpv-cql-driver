#pragma once
#include "../ProtocolTypes/CqlProtocolShortBytes.hpp"
#include "../ProtocolTypes/CqlProtocolQueryParameters.hpp"
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Executes a prepared query
	 * The server will respond a RESULT message, the content of which depends on the query.
	 */
	class CqlExecuteMessage : public CqlRequestMessageBase {
	public:
		using CqlRequestMessageBase::freeResources;

		/** For CqlObject */
		void reset(CqlMessageHeader&& header);

		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;

		/** The query id returned from prepare result */
		const CqlProtocolShortBytes& getPreparedQueryId() const& { return preparedQueryId_; }
		CqlProtocolShortBytes& getPreparedQueryId() & { return preparedQueryId_; }

		/** The query parameters same as QUERY message */
		const CqlProtocolQueryParameters& getQueryParameters() const& { return queryParameters_; }
		CqlProtocolQueryParameters& getQueryParameters() & { return queryParameters_; }

		/** Constructor */
		CqlExecuteMessage();

	private:
		CqlProtocolShortBytes preparedQueryId_;
		CqlProtocolQueryParameters queryParameters_;
	};
}

