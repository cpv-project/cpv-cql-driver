#pragma once
#include "../ProtocolTypes/CqlProtocolByte.hpp"
#include "../ProtocolTypes/CqlProtocolBatchQueryList.hpp"
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

		/** Encode message body to binary data */
		void encodeBody(const CqlConnectionInfo& info, seastar::sstring& data) const override;

		/** Indicating the type of batch to use, can be either "Logged", "UnLogged", or "Counter" */
		CqlBatchType getType() const;
		void setType(CqlBatchType type);

		/** The queries to execute */
		const CqlProtocolBatchQueryList& getQueries() const& { return queries_; }
		CqlProtocolBatchQueryList& getQueries() & { return queries_; }

		/** The batch parameters */
		const CqlProtocolBatchParameters& getBatchParameters() const& { return batchParameters_; }
		CqlProtocolBatchParameters& getBatchParameters() & { return batchParameters_; }

		/** Constructor */
		CqlBatchMessage();

	private:
		CqlProtocolByte type_;
		CqlProtocolBatchQueryList queries_;
		CqlProtocolBatchParameters batchParameters_;
	};
}

