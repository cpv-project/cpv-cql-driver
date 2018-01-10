#include <array>
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>

namespace cql {
	/** Write the text description of consistency level to stream */
	std::ostream& operator<<(std::ostream& stream, CqlConsistencyLevel level) {
		static auto staticNames = [] {
			std::array<const char*, static_cast<std::size_t>(CqlConsistencyLevel::Max_)> names;
			names[static_cast<std::size_t>(CqlConsistencyLevel::Any)] = "Any";
			names[static_cast<std::size_t>(CqlConsistencyLevel::One)] = "One";
			names[static_cast<std::size_t>(CqlConsistencyLevel::Two)] = "Two";
			names[static_cast<std::size_t>(CqlConsistencyLevel::Three)] = "Three";
			names[static_cast<std::size_t>(CqlConsistencyLevel::Quorum)] = "Quorum";
			names[static_cast<std::size_t>(CqlConsistencyLevel::All)] = "All";
			names[static_cast<std::size_t>(CqlConsistencyLevel::LocalQuorum)] = "LocalQuorum";
			names[static_cast<std::size_t>(CqlConsistencyLevel::EachQuorum)] = "EachQuorum";
			names[static_cast<std::size_t>(CqlConsistencyLevel::Serial)] = "Serial";
			names[static_cast<std::size_t>(CqlConsistencyLevel::LocalSerial)] = "LocalSerial";
			names[static_cast<std::size_t>(CqlConsistencyLevel::LocalOne)] = "LocalOne";
			return names;
		}();
		auto index = static_cast<std::size_t>(level);
		if (index >= staticNames.size() || staticNames[index] == nullptr) {
			stream << "Unknow";
		} else {
			stream << staticNames[index];
		}
		return stream;
	}

	/** Write the text description of column type to stream */
	std::ostream& operator<<(std::ostream& stream, CqlColumnType type) {
		static auto staticNames = [] {
			std::array<const char*, static_cast<std::size_t>(CqlColumnType::Max_)> names;
			names[static_cast<std::size_t>(CqlColumnType::Custom)] = "Custom";
			names[static_cast<std::size_t>(CqlColumnType::Ascii)] = "Ascii";
			names[static_cast<std::size_t>(CqlColumnType::BigInt)] = "BigInt";
			names[static_cast<std::size_t>(CqlColumnType::Blob)] = "Blob";
			names[static_cast<std::size_t>(CqlColumnType::Boolean)] = "Boolean";
			names[static_cast<std::size_t>(CqlColumnType::Counter)] = "Counter";
			names[static_cast<std::size_t>(CqlColumnType::Decimal)] = "Decimal";
			names[static_cast<std::size_t>(CqlColumnType::Double)] = "Double";
			names[static_cast<std::size_t>(CqlColumnType::Float)] = "Float";
			names[static_cast<std::size_t>(CqlColumnType::Int)] = "Int";
			names[static_cast<std::size_t>(CqlColumnType::Timestamp)] = "Timestamp";
			names[static_cast<std::size_t>(CqlColumnType::Uuid)] = "Uuid";
			names[static_cast<std::size_t>(CqlColumnType::VarChar)] = "VarChar";
			names[static_cast<std::size_t>(CqlColumnType::VarInt)] = "VarInt";
			names[static_cast<std::size_t>(CqlColumnType::TimeUuid)] = "TimeUuid";
			names[static_cast<std::size_t>(CqlColumnType::Inet)] = "Inet";
			names[static_cast<std::size_t>(CqlColumnType::Date)] = "Date";
			names[static_cast<std::size_t>(CqlColumnType::Time)] = "Time";
			names[static_cast<std::size_t>(CqlColumnType::SmallInt)] = "SmallInt";
			names[static_cast<std::size_t>(CqlColumnType::TinyInt)] = "TinyInt";
			names[static_cast<std::size_t>(CqlColumnType::Duration)] = "Duration";
			names[static_cast<std::size_t>(CqlColumnType::List)] = "List";
			names[static_cast<std::size_t>(CqlColumnType::Map)] = "Map";
			names[static_cast<std::size_t>(CqlColumnType::Set)] = "Set";
			names[static_cast<std::size_t>(CqlColumnType::Udt)] = "Udt";
			names[static_cast<std::size_t>(CqlColumnType::Tuple)] = "Tuple";
			return names;
		}();
		auto index = static_cast<std::size_t>(type);
		if (index >= staticNames.size() || staticNames[index] == nullptr) {
			stream << "Unknow";
		} else {
			stream << staticNames[index];
		}
		return stream;
	}
}

