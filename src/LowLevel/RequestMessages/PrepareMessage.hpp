#pragma once
#include "../ProtocolTypes/ProtocolLongString.hpp"
#include "./RequestMessageBase.hpp"

namespace cql {
	/**
	 * Prepare a query for later execution (through EXECUTE)
	 * The server will respond with a RESULT message with a "prepared" kind.
	 */
	class PrepareMessage : public RequestMessageBase {
	public:
		using RequestMessageBase::freeResources;

		/** For Object */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		std::string toString() const override;

		/** Encode message body to binary data */
		void encodeBody(const ConnectionInfo& info, std::string& data) const override;

		/** The CQL query string */
		const ProtocolLongString& getQuery() const& { return query_; }
		ProtocolLongString& getQuery() & { return query_; }

		/** Constructor */
		PrepareMessage();

	private:
		ProtocolLongString query_;
	};
}

