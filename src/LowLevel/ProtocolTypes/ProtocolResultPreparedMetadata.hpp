#pragma once
#include "../LowLevelDefinitions.hpp"
#include "./ProtocolInt.hpp"
#include "./ProtocolString.hpp"
#include "./ProtocolResultColumn.hpp"

namespace cql {
	/**
	 * <flags><columns_count><pk_count>[<pk_index_1>...<pk_index_n>] ~
	 * ~ [<global_table_spec>?<col_spec_1>...<col_spec_n>]
	 * [] mean optional, depends on <flags>.
	 * If all partition keys presented pk_count will be greater than 0,
	 * a <pk_index> sequence of [2, 0, 1] indicates that the table has three partition key columns,
	 * the full partition key can be constructed by creating a composite of the values for
	 * the bind markers at index 2, at index 0, and at index 1,
	 * this allow implementations with token-aware routing to correctly construct
	 * the partition key without needing to inspect table.
	 * Check native_protocol_v4.spec section 4.2.5.4.
	 */
	class ProtocolResultPreparedMetadata {
	public:
		/** Reset to initial state */
		void reset();

		/** Call setters below will alter flags to indicate which component is included */
		ResultPreparedMetadataFlags getFlags() const {
			return static_cast<ResultPreparedMetadataFlags>(flags_.get());
		}

		/** The partition key column positions, only available if all partition keys presented */
		const std::vector<ProtocolInt>& getPartitionKeyIndexes() const& { return partitionKeyIndexes_; }
		std::vector<ProtocolInt>& getPartitionKeyIndexes() & { return partitionKeyIndexes_; }
		void setPartitionKeyIndexes(const std::vector<ProtocolInt>& partitionKeyIndexes) {
			partitionKeyIndexes_ = partitionKeyIndexes;
		}
		void setPartitionKeyIndexes(std::vector<ProtocolInt>&& partitionKeyIndexes) {
			partitionKeyIndexes_ = std::move(partitionKeyIndexes);
		}

		/** The keyspace name and table name of all the columns belong to */
		const seastar::sstring& getGlobalKeySpace() const& { return globalKeySpace_.get(); }
		const seastar::sstring& getGlobalTable() const& { return globalTable_.get(); }
		void setGlobalKeySpaceAndTable(const seastar::sstring& keySpace, const seastar::sstring& table);
		void setGlobalKeySpaceAndTable(seastar::sstring&& keySpace, seastar::sstring&& table);

		/** The columns selected by the query that produced this result */
		const std::vector<ProtocolResultColumn>& getColumns() const& { return columns_; }
		std::vector<ProtocolResultColumn>& getColumns() & { return columns_; }
		void setColumns(const std::vector<ProtocolResultColumn>& columns) { columns_ = columns; }
		void setColumns(std::vector<ProtocolResultColumn>&& columns) { columns_ = std::move(columns); }

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructor */
		ProtocolResultPreparedMetadata();

	private:
		ProtocolInt flags_;
		std::vector<ProtocolInt> partitionKeyIndexes_;
		ProtocolString globalKeySpace_;
		ProtocolString globalTable_;
		std::vector<ProtocolResultColumn> columns_;
	};
}

