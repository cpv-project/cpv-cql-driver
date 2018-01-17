#pragma once
#include "../CqlLowLevelDefinitions.hpp"
#include "CqlProtocolByte.hpp"
#include "CqlProtocolLongString.hpp"
#include "CqlProtocolShortBytes.hpp"
#include "CqlProtocolValueList.hpp"

namespace cql {
	/**
	 * <kind><string_or_id><n>[<name_1>]<value_1>...[<name_n>]<value_n>
	 * According to CASSANDRA-10246 named parameter support in batch query is broken,
	 * so setNameAndValues is not provided.
	 * Check native_protocol_v4.spec section 4.1.7.
	 */
	class CqlProtocolBatchQuery {
	public:
		/** Reset to initial state */
		void reset();

		/** Indicating the following query is a prepared one or not */
		CqlBatchQueryKind getKind() const;
		
		/** The CQL query string */
		const seastar::sstring& getQuery() const&;
		void setQuery(const seastar::sstring& query);
		void setQuery(seastar::sstring&& query);

		/** The query ID returned from prepare result */
		const seastar::sstring& getPreparedQueryId() const&;
		void setPreparedQueryId(const seastar::sstring& preparedQueryId);
		void setPreparedQueryId(seastar::sstring&& preparedQueryId);

		/** Query parameter values */
		const std::vector<CqlProtocolValue>& getValues() const&;
		void setValues(const std::vector<CqlProtocolValue>& values);
		void setValues(std::vector<CqlProtocolValue>&& values);

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructor */
		CqlProtocolBatchQuery();

	private:
		CqlProtocolByte kind_;
		CqlProtocolLongString query_;
		CqlProtocolShortBytes preparedQueryId_;
		CqlProtocolValueList values_;
	};
}

