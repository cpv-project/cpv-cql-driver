#pragma once
#include "../ProtocolTypes/ProtocolQueryParameters.hpp"
#include "./RequestMessageBase.hpp"

namespace cql {
	/**
	 * Performs a CQL query
	 * The server will respond a RESULT message, the content of which depends on the query.
	 */
	class QueryMessage : public RequestMessageBase {
	public:
		using RequestMessageBase::freeResources;

		/** For Object */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Encode message body to binary data */
		void encodeBody(const ConnectionInfo& info, seastar::sstring& data) const override;

		/** The query parameters */
		const ProtocolQueryParameters& getQueryParameters() const& { return queryParameters_; }
		ProtocolQueryParameters& getQueryParameters() & { return queryParameters_; }

		/** Constructor */
		QueryMessage();

	private:
		ProtocolQueryParameters queryParameters_;
	};
}

