#pragma once
#include <vector>
#include "../ProtocolTypes/ProtocolByte.hpp"
#include "../ProtocolTypes/ProtocolBatchParameters.hpp"
#include "./RequestMessageBase.hpp"

namespace cql {
	/**
	 * Allows executing a list of queries (prepared or not) as a batch
	 * The server will respond with a RESULT message.
	 */
	class BatchMessage : public RequestMessageBase {
	public:
		using RequestMessageBase::freeResources;

		/** For Object */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		std::string toString() const override;

		/** Encode message body to binary data */
		void encodeBody(const ConnectionInfo& info, std::string& data) const override;

		/** The type of batch, can be either "Logged", "UnLogged", or "Counter" */
		BatchType getType() const;
		void setType(BatchType type);

		/** The batch parameters */
		const ProtocolBatchParameters& getBatchParameters() const& { return batchParameters_; }
		ProtocolBatchParameters& getBatchParameters() & { return batchParameters_; }

		/** The prepared query id of the query at the specificed index */
		const std::string& getPreparedQueryId(std::size_t index) const&;
		std::string& getPreparedQueryId(std::size_t index) &;

		/** Constructor */
		BatchMessage();

	private:
		ProtocolByte type_;
		ProtocolBatchParameters batchParameters_;
		std::vector<std::string> preparedQueryIds_;
	};
}

