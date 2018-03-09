#pragma once
#include "../ProtocolTypes/ProtocolShortBytes.hpp"
#include "../ProtocolTypes/ProtocolQueryParameters.hpp"
#include "./RequestMessageBase.hpp"

namespace cql {
	/**
	 * Executes a prepared query
	 * The server will respond a RESULT message, the content of which depends on the query.
	 */
	class ExecuteMessage : public RequestMessageBase {
	public:
		using RequestMessageBase::freeResources;

		/** For Object */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Encode message body to binary data */
		void encodeBody(const ConnectionInfo& info, seastar::sstring& data) const override;

		/** The query id returned from prepare result */
		const ProtocolShortBytes& getPreparedQueryId() const& { return preparedQueryId_; }
		ProtocolShortBytes& getPreparedQueryId() & { return preparedQueryId_; }

		/** The query parameters same as QUERY message */
		const ProtocolQueryParameters& getQueryParameters() const& { return queryParameters_; }
		ProtocolQueryParameters& getQueryParameters() & { return queryParameters_; }

		/** Constructor */
		ExecuteMessage();

	private:
		ProtocolShortBytes preparedQueryId_;
		ProtocolQueryParameters queryParameters_;
	};
}

