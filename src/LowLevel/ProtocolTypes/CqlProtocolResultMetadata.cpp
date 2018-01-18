#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlProtocolResultMetadata.hpp"

namespace cql {
	/** Reset to initial state */
	void CqlProtocolResultMetadata::reset() {
		flags_.set(enumValue(CqlResultMetadataFlags::NoMetadata));
		columnsCount_.reset();
	}

	/** Set the value used to retrieve the next page of results, also indicates there are more pages */
	void CqlProtocolResultMetadata::setPagingState(const seastar::sstring& pagingState) {
		pagingState_.set(pagingState);
		flags_.set(enumValue(getFlags() | CqlResultMetadataFlags::HasMorePages));
	}

	/** Set the value used to retrieve the next page of results, also indicates there are more pages */
	void CqlProtocolResultMetadata::setPagingState(seastar::sstring&& pagingState) {
		pagingState_.set(std::move(pagingState));
		flags_.set(enumValue(getFlags() | CqlResultMetadataFlags::HasMorePages));
	}

	/** Set the keyspace name and table name of all the columns belong to */
	void CqlProtocolResultMetadata::setGlobalKeySpaceAndTable(
		const seastar::sstring& keySpace, const seastar::sstring& table) {
		globalKeySpace_.set(keySpace);
		globalTable_.set(table);
		flags_.set(enumValue(getFlags() | CqlResultMetadataFlags::GlobalTableSpec));
	}

	/** Set the keyspace name and table name of all the columns belong to */
	void CqlProtocolResultMetadata::setGlobalKeySpaceAndTable(
		seastar::sstring&& keySpace, seastar::sstring&& table) {
		globalKeySpace_.set(std::move(keySpace));
		globalTable_.set(std::move(table));
		flags_.set(enumValue(getFlags() | CqlResultMetadataFlags::GlobalTableSpec));
	}

	/** The columns selected by the query that produced this result */
	void CqlProtocolResultMetadata::setColumns(const std::vector<CqlProtocolResultColumn>& columns) {
		columnsCount_.set(columns_.size());
		columns_ = columns;
		flags_.set(enumValue(getFlags() & ~CqlResultMetadataFlags::NoMetadata));
	}

	/** The columns selected by the query that produced this result */
	void CqlProtocolResultMetadata::setColumns(std::vector<CqlProtocolResultColumn>&& columns) {
		columnsCount_.set(columns_.size());
		columns_ = std::move(columns);
		flags_.set(enumValue(getFlags() & ~CqlResultMetadataFlags::NoMetadata));
	}

	/** Encode to binary data */
	void CqlProtocolResultMetadata::encode(seastar::sstring& data) const {
		auto flags = getFlags();
		flags_.encode(data);
		columnsCount_.encode(data);
		if (enumTrue(flags & CqlResultMetadataFlags::HasMorePages)) {
			pagingState_.encode(data);
		}
		if (enumFalse(flags & CqlResultMetadataFlags::NoMetadata)) {
			if (getColumnsCount() != columns_.size()) {
				throw CqlLogicException(CQL_CODEINFO, "columns count != columns.size()");
			}
			if (enumTrue(flags & CqlResultMetadataFlags::GlobalTableSpec)) {
				globalKeySpace_.encode(data);
				globalTable_.encode(data);
			}
			bool columnContainsTableSpec = enumFalse(flags & CqlResultMetadataFlags::GlobalTableSpec);
			for (const auto& column : columns_) {
				column.encode(data, columnContainsTableSpec);
			}
		}
	}

	/** Decode from binary data */
	void CqlProtocolResultMetadata::decode(const char*& ptr, const char* end) {
		flags_.decode(ptr, end);
		auto flags = getFlags();
		columnsCount_.decode(ptr, end);
		if (enumTrue(flags & CqlResultMetadataFlags::HasMorePages)) {
			pagingState_.decode(ptr, end);
		}
		if (enumFalse(flags & CqlResultMetadataFlags::NoMetadata)) {
			if (columnsCount_.get() < 0) {
				throw CqlLogicException(CQL_CODEINFO, "columns count < 0");
			}
			if (columns_.size() > columnsCount_.get()) {
				columns_.resize(columnsCount_.get());
			}
			bool columnContainsTableSpec = enumFalse(flags & CqlResultMetadataFlags::GlobalTableSpec);
			for (std::size_t i = 0, j = columnsCount_.get(); i < j; ++i) {
				if (i < columns_.size()) {
					columns_[i].decode(ptr, end, columnContainsTableSpec);
				} else {
					columns_.emplace_back();
					columns_.back().decode(ptr, end, columnContainsTableSpec);
				}
			}
		}
	}

	/** Constructor */
	CqlProtocolResultMetadata::CqlProtocolResultMetadata() :
		flags_(enumValue(CqlResultMetadataFlags::NoMetadata)),
		columnsCount_(),
		pagingState_(),
		globalKeySpace_(),
		globalTable_(),
		columns_() { }
}

