#pragma once
#include "../ProtocolTypes/ProtocolInt.hpp"
#include "../ProtocolTypes/ProtocolResultRowsMetadata.hpp"
#include "../ProtocolTypes/ProtocolBytes.hpp"
#include "../ProtocolTypes/ProtocolString.hpp"
#include "../ProtocolTypes/ProtocolShortBytes.hpp"
#include "../ProtocolTypes/ProtocolResultPreparedMetadata.hpp"
#include "./ResponseMessageBase.hpp"

namespace cql {
	/** The result to a query (QUERY, PREPARE, EXECUTE or BATCH message */
	class ResultMessage : public ResponseMessageBase {
	public:
		using ResponseMessageBase::freeResources;

		/** Reset to initial state */
		void reset(MessageHeader&& header);

		/** Get description of this message */
		seastar::sstring toString() const override;

		/** Decode message body from binary data */
		void decodeBody(const ConnectionInfo& info, seastar::temporary_buffer<char>&& buffer) override;

		/**
		 * TODO:
		 * Implement a decode function that takes the result entities and able to fill them directly,
		 * to make less memory copy overhead.
		 *
		 * It requires the high level design to be clear.
		 */

		/** Get the kind of result */
		ResultKind getKind() const { return static_cast<ResultKind>(kind_.get()); }

		/** [For Kind Rows] Get the metadata of rows result */
		const ProtocolResultRowsMetadata& getRowsMetadata() const& { return rowsMetadata_; }
		ProtocolResultRowsMetadata& getRowsMetadata() & { return rowsMetadata_; }

		/** [For Kind Rows] Get how many rows present in this result */
		const ProtocolInt& getRowsCount() const& { return rowsCount_; }
		ProtocolInt& getRowsCount() & { return rowsCount_; }

		/** [For Kind Rows] Get the contents of rows, there <rows_count> * <columns_count> [bytes] */
		const std::vector<ProtocolBytes>& getRowsContents() const& { return rowsContents_; }
		std::vector<ProtocolBytes>& getRowsContents() & { return rowsContents_; }

		/** [For Kind SetKeySpace] Get the keyspace that has been set(use) */
		const ProtocolString& getKeySpaceSet() const& { return keySpaceSet_; }
		ProtocolString& getKeySpaceSet() & { return keySpaceSet_; }

		/** [For Kind Prepared] Get the prepared query id */
		const ProtocolShortBytes& getPreparedQueryId() const& { return preparedQueryId_; }
		ProtocolShortBytes& getPreparedQueryId() & { return preparedQueryId_; }

		/** [For Kind Prepared] Get the prepared metadata contains bind markers information */
		const ProtocolResultPreparedMetadata& getPreparedMetadata() const& { return preparedMetadata_; }
		ProtocolResultPreparedMetadata& getPreparedMetadata() & { return preparedMetadata_; }

		/** [For Kind Prepared] Get the prepared rows metadata contains result set information */
		const ProtocolResultRowsMetadata& getPreparedRowsMetadata() const& { return preparedRowsMetadata_; }
		ProtocolResultRowsMetadata& getPreparedRowsMetadata() & { return preparedRowsMetadata_; }

		/** [For Kind SchemaChange] Get the schema change type */
		const ProtocolString& getSchemaChangeType() const& { return schemaChangeType_; }
		ProtocolString& getSchemaChangeType() & { return schemaChangeType_; }

		/** [For Kind SchemaChange] Get the schema change target */
		const ProtocolString& getSchemaChangeTarget() const& { return schemaChangeTarget_; }
		ProtocolString& getSchemaChangeTarget() & { return schemaChangeTarget_; }

		/** [For Kind SchemaChange] Get the schema change options, depends on schema change type */
		const seastar::sstring& getSchemaChangeOptions() const& { return schemaChangeOptions_; }
		seastar::sstring& getSchemaChangeOptions() & { return schemaChangeOptions_; }

		/** Constructor */
		ResultMessage();

	private:
		ProtocolInt kind_;
		// For Kind Rows
		ProtocolResultRowsMetadata rowsMetadata_;
		ProtocolInt rowsCount_;
		std::vector<ProtocolBytes> rowsContents_;
		// For Kind SetKeySpace
		ProtocolString keySpaceSet_;
		// For Kind Prepared
		ProtocolShortBytes preparedQueryId_;
		ProtocolResultPreparedMetadata preparedMetadata_;
		ProtocolResultRowsMetadata preparedRowsMetadata_;
		// For Kind SchemaChange
		ProtocolString schemaChangeType_;
		ProtocolString schemaChangeTarget_;
		seastar::sstring schemaChangeOptions_;
	};
}

