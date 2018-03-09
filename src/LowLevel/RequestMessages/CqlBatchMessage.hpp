#pragma once
#include <vector>
#include "../ProtocolTypes/CqlProtocolByte.hpp"
#include "../ProtocolTypes/CqlProtocolBatchParameters.hpp"
#include "CqlRequestMessageBase.hpp"

namespace cql {
	/**
	 * Allows executing a list of queries (prepared or not) as a batch
	 * The server will respond with a RESULT message.
	 */
	class CqlBatchMessage : public CqlRequestMessageBase {
	public:
		using CqlRequestMessageBase::freeResources;

		/** For CqlObject */
		void reset(CqlMessageHeader&& header);

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;

		/** The type of batch, can be either "Logged", "UnLogged", or "Counter" */
		CqlBatchType getType() const;
		void setType(CqlBatchType type);

		/** The batch parameters */
		const CqlProtocolBatchParameters& getBatchParameters() const& { return batchParameters_; }
		CqlProtocolBatchParameters& getBatchParameters() & { return batchParameters_; }

		/** The prepared query id of the query at the specificed index */
		const seastar::sstring& getPreparedQueryId(std::size_t index) const&;
		seastar::sstring& getPreparedQueryId(std::size_t index) &;

		/** Constructor */
		CqlBatchMessage();

	private:
		CqlProtocolByte type_;
		CqlProtocolBatchParameters batchParameters_;
		std::vector<seastar::sstring> preparedQueryIds_;
	};
}

