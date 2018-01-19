#pragma once
#include "../ProtocolTypes/CqlProtocolInt.hpp"
#include "../ProtocolTypes/CqlProtocolResultRowsMetadata.hpp"
#include "../ProtocolTypes/CqlProtocolBytes.hpp"
#include "../ProtocolTypes/CqlProtocolString.hpp"
#include "../ProtocolTypes/CqlProtocolShortBytes.hpp"
#include "../ProtocolTypes/CqlProtocolResultPreparedMetadata.hpp"
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/** The result to a query (QUERY, PREPARE, EXECUTE or BATCH message */
	class CqlResultMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;

		/** Reset to initial state */
		void reset(CqlMessageHeader&& header);

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const char*& ptr, const char* end) override;

		/**
		 * TODO:
		 * Implement a decode function that takes the result entities and able to fill them directly,
		 * to make less memory copy overhead.
		 *
		 * It requires the high level design to be clear.
		 */

		/** Get the kind of result */
		CqlResultKind getKind() const { return static_cast<CqlResultKind>(kind_.get()); }

		/** [For Kind Rows] Get the metadata of rows result */
		const CqlProtocolResultRowsMetadata& getRowsMetadata() const& { return rowsMetadata_; }
		CqlProtocolResultRowsMetadata& getRowsMetadata() & { return rowsMetadata_; }

		/** [For Kind Rows] Get how many rows present in this result */
		const CqlProtocolInt& getRowsCount() const& { return rowsCount_; }
		CqlProtocolInt& getRowsCount() & { return rowsCount_; }

		/** [For Kind Rows] Get the contents of rows, there <rows_count> * <columns_count> [bytes] */
		const std::vector<CqlProtocolBytes>& getRowsContents() const& { return rowsContents_; }
		std::vector<CqlProtocolBytes>& getRowsContents() & { return rowsContents_; }

		/** [For Kind SetKeySpace] Get the keyspace that has been set(use) */
		const CqlProtocolString& getKeySpaceSet() const& { return keySpaceSet_; }
		CqlProtocolString& getKeySpaceSet() & { return keySpaceSet_; }

		/** [For Kind Prepared] Get the prepared query id */
		const CqlProtocolShortBytes& getPreparedQueryId() const& { return preparedQueryId_; }
		CqlProtocolShortBytes& getPreparedQueryId() & { return preparedQueryId_; }

		/** [For Kind Prepared] Get the prepared metadata contains bind markers information */
		const CqlProtocolResultPreparedMetadata& getPreparedMetadata() const& { return preparedMetadata_; }
		CqlProtocolResultPreparedMetadata& getPreparedMetadata() & { return preparedMetadata_; }

		/** [For Kind Prepared] Get the prepared rows metadata contains result set information */
		const CqlProtocolResultRowsMetadata& getPreparedRowsMetadata() const& { return preparedRowsMetadata_; }
		CqlProtocolResultRowsMetadata& getPreparedRowsMetadata() & { return preparedRowsMetadata_; }

		/** [For Kind SchemaChange] Get the schema change type */
		const CqlProtocolString& getSchemaChangeType() const& { return schemaChangeType_; }
		CqlProtocolString& getSchemaChangeType() & { return schemaChangeType_; }

		/** [For Kind SchemaChange] Get the schema change target */
		const CqlProtocolString& getSchemaChangeTarget() const& { return schemaChangeTarget_; }
		CqlProtocolString& getSchemaChangeTarget() & { return schemaChangeTarget_; }

		/** [For Kind SchemaChange] Get the schema change options, depends on schema change type */
		const seastar::sstring& getSchemaChangeOptions() const& { return schemaChangeOptions_; }
		seastar::sstring& getSchemaChangeOptions() & { return schemaChangeOptions_; }

		/** Constructor */
		CqlResultMessage();

	private:
		CqlProtocolInt kind_;
		// For Kind Rows
		CqlProtocolResultRowsMetadata rowsMetadata_;
		CqlProtocolInt rowsCount_;
		std::vector<CqlProtocolBytes> rowsContents_;
		// For Kind SetKeySpace
		CqlProtocolString keySpaceSet_;
		// For Kind Prepared
		CqlProtocolShortBytes preparedQueryId_;
		CqlProtocolResultPreparedMetadata preparedMetadata_;
		CqlProtocolResultRowsMetadata preparedRowsMetadata_;
		// For Kind SchemaChange
		CqlProtocolString schemaChangeType_;
		CqlProtocolString schemaChangeTarget_;
		seastar::sstring schemaChangeOptions_;
	};
}

