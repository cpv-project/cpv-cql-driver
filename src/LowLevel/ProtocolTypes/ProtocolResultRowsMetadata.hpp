#pragma once
#include "../LowLevelDefinitions.hpp"
#include "./ProtocolInt.hpp"
#include "./ProtocolBytes.hpp"
#include "./ProtocolString.hpp"
#include "./ProtocolResultColumn.hpp"

namespace cql {
	/**
	 * <flags><columns_count>[<paging_state>][<global_table_spec>?<col_spec_1>...<col_spec_n>]
	 * [] mean optional, depends on <flags>.
	 * If <flags> contains NoMetadata, only <columns_count> will available,
	 * and [<global_table_spec>?<col_spec_1>...<col_spec_n>] will not present.
	 * Check native_protocol_v4.spec section 4.2.5.2.
	 */
	class ProtocolResultRowsMetadata {
	public:
		/** Reset to initial state */
		void reset();

		/** Call setters below will alter flags to indicate which component is included */
		ResultRowsMetadataFlags getFlags() const {
			return static_cast<ResultRowsMetadataFlags>(flags_.get());
		}

		/** How many columns selected by the query that produced this result */
		std::size_t getColumnsCount() const { return columnsCount_.get(); }
		void setColumnsCount(std::size_t columnsCount) { columnsCount_.set(columnsCount); }

		/** The value used to retrieve the next page of results */
		const seastar::sstring& getPagingState() const& { return pagingState_.get(); }
		seastar::sstring& getPagingState() & { return pagingState_.get(); }
		void setPagingState(const seastar::sstring& pagingState);
		void setPagingState(seastar::sstring&& pagingState);

		/** The keyspace name and table name of all the columns belong to */
		const seastar::sstring& getGlobalKeySpace() const& { return globalKeySpace_.get(); }
		const seastar::sstring& getGlobalTable() const& { return globalTable_.get(); }
		void setGlobalKeySpaceAndTable(const seastar::sstring& keySpace, const seastar::sstring& table);
		void setGlobalKeySpaceAndTable(seastar::sstring&& keySpace, seastar::sstring&& table);

		/** The columns selected by the query that produced this result */
		const std::vector<ProtocolResultColumn>& getColumns() const& { return columns_; }
		std::vector<ProtocolResultColumn>& getColumns() & { return columns_; }
		void setColumns(const std::vector<ProtocolResultColumn>& columns);
		void setColumns(std::vector<ProtocolResultColumn>&& columns);

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructor */
		ProtocolResultRowsMetadata();

	private:
		ProtocolInt flags_;
		ProtocolInt columnsCount_;
		ProtocolBytes pagingState_;
		ProtocolString globalKeySpace_;
		ProtocolString globalTable_;
		std::vector<ProtocolResultColumn> columns_;
	};
}

