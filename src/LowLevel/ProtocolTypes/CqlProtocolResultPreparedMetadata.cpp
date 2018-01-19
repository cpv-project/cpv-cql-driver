#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlProtocolResultPreparedMetadata.hpp"

namespace cql {
	/** Reset to initial state */
	void CqlProtocolResultPreparedMetadata::reset() {
		flags_.set(enumValue(CqlResultPreparedMetadataFlags::None));
		partitionKeyIndexes_.clear();
		globalKeySpace_.reset();
		globalTable_.reset();
		columns_.clear();
	}

	/** Set the keyspace name and table name of all the columns belong to */
	void CqlProtocolResultPreparedMetadata::setGlobalKeySpaceAndTable(
		const seastar::sstring& keySpace, const seastar::sstring& table) {
		globalKeySpace_.set(keySpace);
		globalTable_.set(table);
		flags_.set(enumValue(getFlags() | CqlResultPreparedMetadataFlags::GlobalTableSpec));
	}

	/** Set the keyspace name and table name of all the columns belong to */
	void CqlProtocolResultPreparedMetadata::setGlobalKeySpaceAndTable(
		seastar::sstring&& keySpace, seastar::sstring&& table) {
		globalKeySpace_.set(std::move(keySpace));
		globalTable_.set(std::move(table));
		flags_.set(enumValue(getFlags() | CqlResultPreparedMetadataFlags::GlobalTableSpec));
	}

	/** Encode to binary data */
	void CqlProtocolResultPreparedMetadata::encode(seastar::sstring& data) const {
		CqlProtocolInt columnsCount(columns_.size());
		CqlProtocolInt partitionKeyIndexesCount(partitionKeyIndexes_.size());
		if (columnsCount.get() < 0 ||
			static_cast<std::size_t>(columnsCount.get()) != columns_.size()) {
			throw CqlLogicException(CQL_CODEINFO, "too many columns cause overflow");
		}
		if (partitionKeyIndexesCount.get() < 0 ||
			static_cast<std::size_t>(partitionKeyIndexesCount.get()) != partitionKeyIndexes_.size()) {
			throw CqlLogicException(CQL_CODEINFO, "too many partition key indexes cause overflow");
		}
		auto flags = getFlags();
		flags_.encode(data);
		columnsCount.encode(data);
		partitionKeyIndexesCount.encode(data);
		for (const auto& partitionKeyIndex : partitionKeyIndexes_) {
			partitionKeyIndex.encode(data);
		}
		bool columnContainsTableSpec = true;
		if (enumTrue(flags & CqlResultPreparedMetadataFlags::GlobalTableSpec)) {
			globalKeySpace_.encode(data);
			globalTable_.encode(data);
			columnContainsTableSpec = false;
		}
		for (const auto& column : columns_) {
			column.encode(data, columnContainsTableSpec);
		}
	}

	/* Decode to binary data */
	void CqlProtocolResultPreparedMetadata::decode(const char*& ptr, const char* end) {
		flags_.decode(ptr, end);
		auto flags = getFlags();
		CqlProtocolInt columnsCount;
		CqlProtocolInt partitionKeyIndexesCount;
		columnsCount.decode(ptr, end);
		partitionKeyIndexesCount.decode(ptr, end);
		if (columnsCount.get() < 0) {
			throw CqlLogicException(CQL_CODEINFO, "columns count < 0");
		}
		if (partitionKeyIndexesCount.get() < 0) {
			throw CqlLogicException(CQL_CODEINFO, "partition key indexes count < 0");
		}
		partitionKeyIndexes_.resize(0);
		for (std::size_t i = 0, j = static_cast<std::size_t>(partitionKeyIndexesCount.get()); i < j; ++i) {
			partitionKeyIndexes_.emplace_back();
			partitionKeyIndexes_.back().decode(ptr, end);
		}
		bool columnContainsTableSpec = true;
		if (enumTrue(flags & CqlResultPreparedMetadataFlags::GlobalTableSpec)) {
			globalKeySpace_.decode(ptr, end);
			globalTable_.decode(ptr, end);
			columnContainsTableSpec = false;
		}
		columns_.resize(0);
		for (std::size_t i = 0, j = static_cast<std::size_t>(columnsCount.get()); i < j; ++i) {
			columns_.emplace_back();
			columns_.back().decode(ptr, end, columnContainsTableSpec);
		}
	}

	/** Constructor */
	CqlProtocolResultPreparedMetadata::CqlProtocolResultPreparedMetadata() :
		flags_(),
		partitionKeyIndexes_(),
		globalKeySpace_(),
		globalTable_(),
		columns_() { }
}

