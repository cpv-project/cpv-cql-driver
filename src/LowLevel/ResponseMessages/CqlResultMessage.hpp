#pragma once
#include "../ProtocolTypes/CqlProtocolInt.hpp"
#include "../ProtocolTypes/CqlProtocolResultRowsMetadata.hpp"
#include "../ProtocolTypes/CqlProtocolBytes.hpp"
#include "../ProtocolTypes/CqlProtocolString.hpp"
// #include "../ProtocolTypes/CqlProtocolResultPreparedMetadata.hpp"
#include "CqlResponseMessageBase.hpp"

namespace cql {
	/** The result to a query (QUERY, PREPARE, EXECUTE or BATCH message */
	class CqlResultMessage : public CqlResponseMessageBase {
	public:
		using CqlResponseMessageBase::freeResources;
		using CqlResponseMessageBase::reset;

		/** Decode message body from binary data */
		void decodeBody(const CqlConnectionInfo& info, const char*& ptr, const char* end) override;

	private:
		CqlProtocolInt kind_;
		// For Kind Rows
		CqlProtocolResultRowsMetadata rowsMetadata_;
		CqlProtocolInt rowsCount_;
		std::vector<CqlProtocolBytes> rowsContents_;
		// For Kind SetKeySpace
		CqlProtocolString keySpaceSet_;
		// For Kind Prepared
		// CqlProtocolResultPreparedMetadata preparedMetadata_;
		CqlProtocolResultRowsMetadata preparedRowsMetadata_;
		// For Kind SchemaChange
		CqlProtocolString schemaChangeType_;
		CqlProtocolString schemaChangeTarget_;
		seastar::sstring schemaChangeOptions_;
	};
}

