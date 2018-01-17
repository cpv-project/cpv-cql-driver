#include <array>
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>

namespace cql {
	const std::vector<std::pair<CqlConsistencyLevel, const char*>>&
		EnumDescriptions<CqlConsistencyLevel>::get() {
		static std::vector<std::pair<CqlConsistencyLevel, const char*>> staticNames({
			{ CqlConsistencyLevel::Any, "Any" },
			{ CqlConsistencyLevel::One, "One" },
			{ CqlConsistencyLevel::Two, "Two" },
			{ CqlConsistencyLevel::Three, "Three" },
			{ CqlConsistencyLevel::Quorum, "Quorum" },
			{ CqlConsistencyLevel::All, "All" },
			{ CqlConsistencyLevel::LocalQuorum, "LocalQuorum" },
			{ CqlConsistencyLevel::EachQuorum, "EachQuorum" },
			{ CqlConsistencyLevel::Serial, "Serial" },
			{ CqlConsistencyLevel::LocalSerial, "LocalSerial" },
			{ CqlConsistencyLevel::LocalOne, "LocalOne" },
		});
		return staticNames;
	}

	const std::vector<std::pair<CqlColumnType, const char*>>&
		EnumDescriptions<CqlColumnType>::get() {
		static std::vector<std::pair<CqlColumnType, const char*>> staticNames({
			{ CqlColumnType::Custom, "Custom" },
			{ CqlColumnType::Ascii, "Ascii" },
			{ CqlColumnType::BigInt, "BigInt" },
			{ CqlColumnType::Blob, "Blob" },
			{ CqlColumnType::Boolean, "Boolean" },
			{ CqlColumnType::Counter, "Counter" },
			{ CqlColumnType::Decimal, "Decimal" },
			{ CqlColumnType::Double, "Double" },
			{ CqlColumnType::Float, "Float" },
			{ CqlColumnType::Int, "Int" },
			{ CqlColumnType::Timestamp, "Timestamp" },
			{ CqlColumnType::Uuid, "Uuid" },
			{ CqlColumnType::VarChar, "VarChar" },
			{ CqlColumnType::VarInt, "VarInt" },
			{ CqlColumnType::TimeUuid, "TimeUuid" },
			{ CqlColumnType::Inet, "Inet" },
			{ CqlColumnType::Date, "Date" },
			{ CqlColumnType::Time, "Time" },
			{ CqlColumnType::SmallInt, "SmallInt" },
			{ CqlColumnType::TinyInt, "TinyInt" },
			{ CqlColumnType::Duration, "Duration" },
			{ CqlColumnType::List, "List" },
			{ CqlColumnType::Map, "Map" },
			{ CqlColumnType::Set, "Set" },
			{ CqlColumnType::Udt, "Udt" },
			{ CqlColumnType::Tuple, "Tuple" },
		});
		return staticNames;
	}

	const std::vector<std::pair<CqlErrorCode, const char*>>&
		EnumDescriptions<CqlErrorCode>::get() {
		static std::vector<std::pair<CqlErrorCode, const char*>> staticNames({
			{ CqlErrorCode::ServerError, "ServerError" },
			{ CqlErrorCode::ProtocolError, "ProtocolError" },
			{ CqlErrorCode::AuthenticationError, "AuthenticationError" },
			{ CqlErrorCode::Unavailable, "Unavailable" },
			{ CqlErrorCode::Overloaded, "Overloaded" },
			{ CqlErrorCode::IsBootstrapping, "IsBootstrapping" },
			{ CqlErrorCode::TruncateError, "TruncateError" },
			{ CqlErrorCode::WriteTimeout, "WriteTimeout" },
			{ CqlErrorCode::ReadTimeout, "ReadTimeout" },
			{ CqlErrorCode::ReadFailure, "ReadFailure" },
			{ CqlErrorCode::FunctionFailure, "FunctionFailure" },
			{ CqlErrorCode::WriteFailure, "WriteFailure" },
			{ CqlErrorCode::SyntaxError, "SyntaxError" },
			{ CqlErrorCode::UnAuthorized, "UnAuthorized" },
			{ CqlErrorCode::InvalidQuery, "InvalidQuery" },
			{ CqlErrorCode::ConfigError, "ConfigError" },
			{ CqlErrorCode::AlreadyExists, "AlreadyExists" },
			{ CqlErrorCode::UnPreparedQuery, "UnPreparedQuery" },
		});
		return staticNames;
	}
}

