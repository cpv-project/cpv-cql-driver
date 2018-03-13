#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include "../../Common/ResultSetData.hpp"
#include "./ResultMessage.hpp"

namespace cql {
	/** Reset to initial state */
	void ResultMessage::reset(MessageHeader&& header) {
		ResponseMessageBase::reset(std::move(header));
		kind_.reset();
		rowsMetadata_.reset();
		rowsCount_.reset();
		resultSet_ = ResultSet(nullptr);
		keySpaceSet_.reset();
		preparedQueryId_.reset();
		preparedMetadata_.reset();
		preparedRowsMetadata_.reset();
		schemaChangeType_.reset();
		schemaChangeTarget_.reset();
		schemaChangeOptions_.resize(0);
	}

	/** Get description of this message */
	seastar::sstring ResultMessage::toString() const {
		return joinString("",
			"ResultMessage(kind: ", getKind(),
			", columnsCount: ", rowsMetadata_.getColumnsCount(),
			", rowsCount: ", rowsCount_.get(), ")");
	}

	/** Decode message body from binary data */
	void ResultMessage::decodeBody(
		const ConnectionInfo&, seastar::temporary_buffer<char>&& buffer) {
		const char* ptr = buffer.begin();
		const char* end = buffer.end();
		kind_.decode(ptr, end);
		auto kind = getKind();
		if (kind == ResultKind::Void) {
			// for results carring no information
		} else if (kind == ResultKind::Rows) {
			// for result to select queries, returning a set of rows
			rowsMetadata_.decode(ptr, end);
			rowsCount_.decode(ptr, end);
			auto rowsCount = static_cast<std::size_t>(rowsCount_.get());
			auto columnsCount = rowsMetadata_.getColumnsCount();
			auto fromOffset = ptr - buffer.begin();
			auto toOffset = end - buffer.begin();
			resultSet_ = ResultSet(makeObject<ResultSetData>(
				rowsCount,
				columnsCount,
				std::move(buffer),
				fromOffset,
				toOffset
			));
		} else if (kind == ResultKind::SetKeySpace) {
			// for result to a "use" query
			keySpaceSet_.decode(ptr, end);
		} else if (kind == ResultKind::Prepared) {
			// for result to a PREPARE message
			preparedQueryId_.decode(ptr, end);
			preparedMetadata_.decode(ptr, end);
			preparedRowsMetadata_.decode(ptr, end);
		} else if (kind == ResultKind::SchemaChange) {
			// for result to a schema altering query
			schemaChangeType_.decode(ptr, end);
			schemaChangeTarget_.decode(ptr, end);
			schemaChangeOptions_.resize(0);
			schemaChangeOptions_.append(ptr, end - ptr);
		} else {
			throw LogicException(CQL_CODEINFO,
				"unsupported result kind:", static_cast<std::size_t>(kind));
		}
	}

	/** Constructor */
	ResultMessage::ResultMessage() :
		kind_(),
		rowsMetadata_(),
		rowsCount_(),
		resultSet_(nullptr),
		keySpaceSet_(),
		preparedQueryId_(),
		preparedMetadata_(),
		preparedRowsMetadata_(),
		schemaChangeType_(),
		schemaChangeTarget_(),
		schemaChangeOptions_() { }
}

