#pragma once
#include "../ProtocolTypes/CqlProtocolLongString.hpp"
#include "../ProtocolTypes/CqlProtocolQueryParameters.hpp"
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Performs a CQL query
	 * The server will respond a RESULT message, the content of which depends on the query.
	 */
	class CqlQueryMessage : public CqlRequestMessageBase {
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

		/** The query parameters */
		const CqlProtocolQueryParameters& getQueryParameters() const& { return queryParameters_; }
		CqlProtocolQueryParameters& getQueryParameters() & { return queryParameters_; }

		/** Constructor */
		CqlQueryMessage();

	private:
		CqlProtocolLongString query_;
		CqlProtocolQueryParameters queryParameters_;
	};
}

