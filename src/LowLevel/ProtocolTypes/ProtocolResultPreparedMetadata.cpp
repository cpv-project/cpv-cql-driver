#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include "./ProtocolResultPreparedMetadata.hpp"

namespace cql {
	/** Reset to initial state */
	void ProtocolResultPreparedMetadata::reset() {
		flags_.set(enumValue(ResultPreparedMetadataFlags::None));
		partitionKeyIndexes_.clear();
		globalKeySpace_.reset();
		globalTable_.reset();
		columns_.clear();
	}

	/** Set the keyspace name and table name of all the columns belong to */
	void ProtocolResultPreparedMetadata::setGlobalKeySpaceAndTable(
		const std::string& keySpace, const std::string& table) {
		globalKeySpace_.set(keySpace);
		globalTable_.set(table);
		flags_.set(enumValue(getFlags() | ResultPreparedMetadataFlags::GlobalTableSpec));
	}

	/** Set the keyspace name and table name of all the columns belong to */
	void ProtocolResultPreparedMetadata::setGlobalKeySpaceAndTable(
		std::string&& keySpace, std::string&& table) {
		globalKeySpace_.set(std::move(keySpace));
		globalTable_.set(std::move(table));
		flags_.set(enumValue(getFlags() | ResultPreparedMetadataFlags::GlobalTableSpec));
	}

	/** Encode to binary data */
	void ProtocolResultPreparedMetadata::encode(std::string& data) const {
		ProtocolInt columnsCount(columns_.size());
		ProtocolInt partitionKeyIndexesCount(partitionKeyIndexes_.size());
		if (columnsCount.get() < 0 ||
			static_cast<std::size_t>(columnsCount.get()) != columns_.size()) {
			throw LogicException(CQL_CODEINFO, "too many columns cause overflow");
		}
		if (partitionKeyIndexesCount.get() < 0 ||
			static_cast<std::size_t>(partitionKeyIndexesCount.get()) != partitionKeyIndexes_.size()) {
			throw LogicException(CQL_CODEINFO, "too many partition key indexes cause overflow");
		}
		auto flags = getFlags();
		flags_.encode(data);
		columnsCount.encode(data);
		partitionKeyIndexesCount.encode(data);
		for (const auto& partitionKeyIndex : partitionKeyIndexes_) {
			partitionKeyIndex.encode(data);
		}
		bool columnContainsTableSpec = true;
		if (enumTrue(flags & ResultPreparedMetadataFlags::GlobalTableSpec)) {
			globalKeySpace_.encode(data);
			globalTable_.encode(data);
			columnContainsTableSpec = false;
		}
		for (const auto& column : columns_) {
			column.encode(data, columnContainsTableSpec);
		}
	}

	/* Decode to binary data */
	void ProtocolResultPreparedMetadata::decode(const char*& ptr, const char* end) {
		flags_.decode(ptr, end);
		auto flags = getFlags();
		ProtocolInt columnsCount;
		ProtocolInt partitionKeyIndexesCount;
		columnsCount.decode(ptr, end);
		partitionKeyIndexesCount.decode(ptr, end);
		if (columnsCount.get() < 0) {
			throw LogicException(CQL_CODEINFO, "columns count < 0");
		}
		if (partitionKeyIndexesCount.get() < 0) {
			throw LogicException(CQL_CODEINFO, "partition key indexes count < 0");
		}
		partitionKeyIndexes_.resize(0);
		for (std::size_t i = 0, j = static_cast<std::size_t>(partitionKeyIndexesCount.get()); i < j; ++i) {
			partitionKeyIndexes_.emplace_back();
			partitionKeyIndexes_.back().decode(ptr, end);
		}
		bool columnContainsTableSpec = true;
		if (enumTrue(flags & ResultPreparedMetadataFlags::GlobalTableSpec)) {
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
	ProtocolResultPreparedMetadata::ProtocolResultPreparedMetadata() :
		flags_(),
		partitionKeyIndexes_(),
		globalKeySpace_(),
		globalTable_(),
		columns_() { }
}

