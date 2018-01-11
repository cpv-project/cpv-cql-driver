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

	/**
	 * Column types
	 * For more information see this page:
	 * http://docs.datastax.com/en/cql/3.1/cql/cql_reference/cql_data_types_c.html
	 */
	enum class CqlColumnType {
		Custom = 0x0000,
		Ascii = 0x0001,
		BigInt = 0x0002,
		Blob = 0x0003,
		Boolean = 0x0004,
		Counter = 0x0005,
		Decimal = 0x0006,
		Double = 0x0007,
		Float = 0x0008,
		Int = 0x0009,
		Timestamp = 0x000B,
		Uuid = 0x000C,
		VarChar = 0x000D,
		VarInt = 0x000E,
		TimeUuid = 0x000F,
		Inet = 0x0010,
		Date = 0x0011,
		Time = 0x0012,
		SmallInt = 0x0013,
		TinyInt = 0x0014,
		Duration = 0x0015,
		List = 0x0020,
		Map = 0x0021,
		Set = 0x0022,
		Udt = 0x0030,
		Tuple = 0x0031,
		Max_ = 0x0032
	};

	/** Write the text description of column type to stream */
	std::ostream& operator<<(std::ostream& stream, CqlColumnType type);

	/** The full class name of the IAuthenticator in use */
	namespace CqlAuthenticatorClasses {
		static const char* AllowAllAuthenticator = "AllowAllAuthenticator";
		static const char* PasswordAuthenticator = "PasswordAuthenticator";
	}
}

