#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlProtocolResultRowsMetadata.hpp"

namespace cql {
	/** Reset to initial state */
	void CqlProtocolResultRowsMetadata::reset() {
		flags_.set(enumValue(CqlResultRowsMetadataFlags::NoMetadata));
		columnsCount_.reset();
	}

	/** Set the value used to retrieve the next page of results, also indicates there are more pages */
	void CqlProtocolResultRowsMetadata::setPagingState(const seastar::sstring& pagingState) {
		pagingState_.set(pagingState);
		flags_.set(enumValue(getFlags() | CqlResultRowsMetadataFlags::HasMorePages));
	}

	/** Set the value used to retrieve the next page of results, also indicates there are more pages */
	void CqlProtocolResultRowsMetadata::setPagingState(seastar::sstring&& pagingState) {
		pagingState_.set(std::move(pagingState));
		flags_.set(enumValue(getFlags() | CqlResultRowsMetadataFlags::HasMorePages));
	}

	/** Set the keyspace name and table name of all the columns belong to */
	void CqlProtocolResultRowsMetadata::setGlobalKeySpaceAndTable(
		const seastar::sstring& keySpace, const seastar::sstring& table) {
		globalKeySpace_.set(keySpace);
		globalTable_.set(table);
		flags_.set(enumValue(getFlags() | CqlResultRowsMetadataFlags::GlobalTableSpec));
	}

	/** Set the keyspace name and table name of all the columns belong to */
	void CqlProtocolResultRowsMetadata::setGlobalKeySpaceAndTable(
		seastar::sstring&& keySpace, seastar::sstring&& table) {
		globalKeySpace_.set(std::move(keySpace));
		globalTable_.set(std::move(table));
		flags_.set(enumValue(getFlags() | CqlResultRowsMetadataFlags::GlobalTableSpec));
	}

	/** The columns selected by the query that produced this result */
	void CqlProtocolResultRowsMetadata::setColumns(const std::vector<CqlProtocolResultColumn>& columns) {
		columnsCount_.set(columns.size());
		columns_ = columns;
		flags_.set(enumValue(getFlags() & ~CqlResultRowsMetadataFlags::NoMetadata));
	}

	/** The columns selected by the query that produced this result */
	void CqlProtocolResultRowsMetadata::setColumns(std::vector<CqlProtocolResultColumn>&& columns) {
		columnsCount_.set(columns.size());
		columns_ = std::move(columns);
		flags_.set(enumValue(getFlags() & ~CqlResultRowsMetadataFlags::NoMetadata));
	}

	/** Encode to binary data */
	void CqlProtocolResultRowsMetadata::encode(seastar::sstring& data) const {
		auto flags = getFlags();
		flags_.encode(data);
		columnsCount_.encode(data);
		if (enumTrue(flags & CqlResultRowsMetadataFlags::HasMorePages)) {
			pagingState_.encode(data);
		}
		if (enumFalse(flags & CqlResultRowsMetadataFlags::NoMetadata)) {
			bool columnContainsTableSpec = true;
			if (enumTrue(flags & CqlResultRowsMetadataFlags::GlobalTableSpec)) {
				globalKeySpace_.encode(data);
				globalTable_.encode(data);
				columnContainsTableSpec = false;
			}
			if (getColumnsCount() != columns_.size()) {
				throw CqlLogicException(CQL_CODEINFO, "columns count != columns.size()");
			}
			for (const auto& column : columns_) {
				column.encode(data, columnContainsTableSpec);
			}
		}
	}

	/** Decode from binary data */
	void CqlProtocolResultRowsMetadata::decode(const char*& ptr, const char* end) {
		flags_.decode(ptr, end);
		auto flags = getFlags();
		columnsCount_.decode(ptr, end);
		if (enumTrue(flags & CqlResultRowsMetadataFlags::HasMorePages)) {
			pagingState_.decode(ptr, end);
		}
		if (enumFalse(flags & CqlResultRowsMetadataFlags::NoMetadata)) {
			bool columnContainsTableSpec = true;
			if (enumTrue(flags & CqlResultRowsMetadataFlags::GlobalTableSpec)) {
				globalKeySpace_.decode(ptr, end);
				globalTable_.decode(ptr, end);
				columnContainsTableSpec = false;
			}
			if (columnsCount_.get() < 0) {
				throw CqlLogicException(CQL_CODEINFO, "columns count < 0");
			}
			if (columns_.size() > columnsCount_.get()) {
				columns_.resize(columnsCount_.get());
			}
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
	CqlProtocolResultRowsMetadata::CqlProtocolResultRowsMetadata() :
		flags_(enumValue(CqlResultRowsMetadataFlags::NoMetadata)),
		columnsCount_(),
		pagingState_(),
		globalKeySpace_(),
		globalTable_(),
		columns_() { }
}
