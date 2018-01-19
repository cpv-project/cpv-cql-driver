#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlResultMessage.hpp"

namespace cql {
	/** Reset to initial state */
	void CqlResultMessage::reset(CqlMessageHeader&& header) {
		CqlResponseMessageBase::reset(std::move(header));
		kind_.reset();
		rowsMetadata_.reset();
		rowsCount_.reset();
		rowsContents_.clear();
		keySpaceSet_.reset();
		preparedQueryId_.reset();
		preparedMetadata_.reset();
		preparedRowsMetadata_.reset();
		schemaChangeType_.reset();
		schemaChangeTarget_.reset();
		schemaChangeOptions_.resize(0);
	}

	/** Decode message body from binary data */
	void CqlResultMessage::decodeBody(const CqlConnectionInfo&, const char*& ptr, const char* end) {
		kind_.decode(ptr, end);
		auto kind = getKind();
		if (kind == CqlResultKind::Void) {
			// for results carring no information
		} else if (kind == CqlResultKind::Rows) {
			// for result to select queries, returning a set of rows
			rowsMetadata_.decode(ptr, end);
			rowsCount_.decode(ptr, end);
			auto rowsCount = static_cast<std::size_t>(rowsCount_.get());
			auto columnsCount = rowsMetadata_.getColumnsCount();
			if (rowsCount_.get() < 0 ||
				std::numeric_limits<std::size_t>::max() / rowsCount < columnsCount) {
				throw CqlLogicException(CQL_CODEINFO,
					"result rows * columns overflow, rows:", rowsCount, "columns", columnsCount);
			}
			auto contentsLength = rowsCount * columnsCount;
			rowsContents_.resize(0);
			for (std::size_t i = 0; i < contentsLength; ++i) {
				rowsContents_.emplace_back();
				rowsContents_.back().decode(ptr, end);
			}
		} else if (kind == CqlResultKind::SetKeySpace) {
			// for result to a "use" query
			keySpaceSet_.decode(ptr, end);
		} else if (kind == CqlResultKind::Prepared) {
			// for result to a PREPARE message
			preparedQueryId_.decode(ptr, end);
			preparedMetadata_.decode(ptr, end);
			preparedRowsMetadata_.decode(ptr, end);
		} else if (kind == CqlResultKind::SchemaChange) {
			// for result to a schema altering query
			schemaChangeType_.decode(ptr, end);
			schemaChangeTarget_.decode(ptr, end);
			schemaChangeOptions_.resize(0);
			schemaChangeOptions_.append(ptr, end - ptr);
			ptr = end;
		} else {
			throw CqlLogicException(CQL_CODEINFO,
				"unsupported result kind:", static_cast<std::size_t>(kind));
		}
	}

	/** Constructor */
	CqlResultMessage::CqlResultMessage() :
		kind_(),
		rowsMetadata_(),
		rowsCount_(),
		rowsContents_(),
		keySpaceSet_(),
		preparedQueryId_(),
		preparedMetadata_(),
		preparedRowsMetadata_(),
		schemaChangeType_(),
		schemaChangeTarget_(),
		schemaChangeOptions_() { }
}

