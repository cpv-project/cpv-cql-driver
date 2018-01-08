#pragma once
#include <ostream>

namespace cql {
	/**
	 * Consistency level for cql query
	 * For more information see this page:
	 * https://docs.datastax.com/en/cassandra/2.1/cassandra/dml/dml_config_consistency_c.html
	 */
	enum class CqlConsistencyLevel {
		Any = 0,
		One = 1,
		Two = 2,
		Three = 3,
		Quorum = 4,
		All = 5,
		LocalQuorum = 6,
		EachQuorum = 7,
		Serial = 8,
		LocalSerial = 9,
		LocalOne = 0xa,
		Max_ = 0xb
	};

	/** Write the text description of consistency level to stream */
	std::ostream& operator<<(std::ostream& stream, CqlConsistencyLevel level);
}

