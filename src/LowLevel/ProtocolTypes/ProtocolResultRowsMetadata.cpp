#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include "./ProtocolResultRowsMetadata.hpp"

namespace cql {
	/** Reset to initial state */
	void ProtocolResultRowsMetadata::reset() {
		flags_.set(enumValue(ResultRowsMetadataFlags::NoMetadata));
		columnsCount_.reset();
	}

	/** Set the value used to retrieve the next page of results, also indicates there are more pages */
	void ProtocolResultRowsMetadata::setPagingState(const seastar::sstring& pagingState) {
		pagingState_.set(pagingState);
		flags_.set(enumValue(getFlags() | ResultRowsMetadataFlags::HasMorePages));
	}

	/** Set the value used to retrieve the next page of results, also indicates there are more pages */
	void ProtocolResultRowsMetadata::setPagingState(seastar::sstring&& pagingState) {
		pagingState_.set(std::move(pagingState));
		flags_.set(enumValue(getFlags() | ResultRowsMetadataFlags::HasMorePages));
	}

	/** Set the keyspace name and table name of all the columns belong to */
	void ProtocolResultRowsMetadata::setGlobalKeySpaceAndTable(
		const seastar::sstring& keySpace, const seastar::sstring& table) {
		globalKeySpace_.set(keySpace);
		globalTable_.set(table);
		flags_.set(enumValue(getFlags() | ResultRowsMetadataFlags::GlobalTableSpec));
	}

	/** Set the keyspace name and table name of all the columns belong to */
	void ProtocolResultRowsMetadata::setGlobalKeySpaceAndTable(
		seastar::sstring&& keySpace, seastar::sstring&& table) {
		globalKeySpace_.set(std::move(keySpace));
		globalTable_.set(std::move(table));
		flags_.set(enumValue(getFlags() | ResultRowsMetadataFlags::GlobalTableSpec));
	}

	/** The columns selected by the query that produced this result */
	void ProtocolResultRowsMetadata::setColumns(const std::vector<ProtocolResultColumn>& columns) {
		columnsCount_.set(columns.size());
		columns_ = columns;
		flags_.set(enumValue(getFlags() & ~ResultRowsMetadataFlags::NoMetadata));
	}

	/** The columns selected by the query that produced this result */
	void ProtocolResultRowsMetadata::setColumns(std::vector<ProtocolResultColumn>&& columns) {
		columnsCount_.set(columns.size());
		columns_ = std::move(columns);
		flags_.set(enumValue(getFlags() & ~ResultRowsMetadataFlags::NoMetadata));
	}

	/** Encode to binary data */
	void ProtocolResultRowsMetadata::encode(seastar::sstring& data) const {
		auto flags = getFlags();
		flags_.encode(data);
		columnsCount_.encode(data);
		if (enumTrue(flags & ResultRowsMetadataFlags::HasMorePages)) {
			pagingState_.encode(data);
		}
		if (enumFalse(flags & ResultRowsMetadataFlags::NoMetadata)) {
			bool columnContainsTableSpec = true;
			if (enumTrue(flags & ResultRowsMetadataFlags::GlobalTableSpec)) {
				globalKeySpace_.encode(data);
				globalTable_.encode(data);
				columnContainsTableSpec = false;
			}
			if (getColumnsCount() != columns_.size()) {
				throw LogicException(CQL_CODEINFO, "columns count != columns.size()");
			}
			for (const auto& column : columns_) {
				column.encode(data, columnContainsTableSpec);
			}
		}
	}

	/** Decode from binary data */
	void ProtocolResultRowsMetadata::decode(const char*& ptr, const char* end) {
		flags_.decode(ptr, end);
		auto flags = getFlags();
		columnsCount_.decode(ptr, end);
		if (enumTrue(flags & ResultRowsMetadataFlags::HasMorePages)) {
			pagingState_.decode(ptr, end);
		}
		if (enumFalse(flags & ResultRowsMetadataFlags::NoMetadata)) {
			bool columnContainsTableSpec = true;
			if (enumTrue(flags & ResultRowsMetadataFlags::GlobalTableSpec)) {
				globalKeySpace_.decode(ptr, end);
				globalTable_.decode(ptr, end);
				columnContainsTableSpec = false;
			}
			if (columnsCount_.get() < 0) {
				throw LogicException(CQL_CODEINFO, "columns count < 0");
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
	ProtocolResultRowsMetadata::ProtocolResultRowsMetadata() :
		flags_(enumValue(ResultRowsMetadataFlags::NoMetadata)),
		columnsCount_(),
		pagingState_(),
		globalKeySpace_(),
		globalTable_(),
		columns_() { }
}

