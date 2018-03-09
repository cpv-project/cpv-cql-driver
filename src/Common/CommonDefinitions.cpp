#include <array>
#include <CQLDriver/Common/CommonDefinitions.hpp>

namespace cql {
	const std::vector<std::pair<ConsistencyLevel, const char*>>&
		EnumDescriptions<ConsistencyLevel>::get() {
		static std::vector<std::pair<ConsistencyLevel, const char*>> staticNames({
			{ ConsistencyLevel::Any, "Any" },
			{ ConsistencyLevel::One, "One" },
			{ ConsistencyLevel::Two, "Two" },
			{ ConsistencyLevel::Three, "Three" },
			{ ConsistencyLevel::Quorum, "Quorum" },
			{ ConsistencyLevel::All, "All" },
			{ ConsistencyLevel::LocalQuorum, "LocalQuorum" },
			{ ConsistencyLevel::EachQuorum, "EachQuorum" },
			{ ConsistencyLevel::Serial, "Serial" },
			{ ConsistencyLevel::LocalSerial, "LocalSerial" },
			{ ConsistencyLevel::LocalOne, "LocalOne" },
		});
		return staticNames;
	}

	const std::vector<std::pair<ColumnType, const char*>>&
		EnumDescriptions<ColumnType>::get() {
		static std::vector<std::pair<ColumnType, const char*>> staticNames({
			{ ColumnType::Custom, "Custom" },
			{ ColumnType::Ascii, "Ascii" },
			{ ColumnType::BigInt, "BigInt" },
			{ ColumnType::Blob, "Blob" },
			{ ColumnType::Boolean, "Boolean" },
			{ ColumnType::Counter, "Counter" },
			{ ColumnType::Decimal, "Decimal" },
			{ ColumnType::Double, "Double" },
			{ ColumnType::Float, "Float" },
			{ ColumnType::Int, "Int" },
			{ ColumnType::Timestamp, "Timestamp" },
			{ ColumnType::UUID, "UUID" },
			{ ColumnType::VarChar, "VarChar" },
			{ ColumnType::VarInt, "VarInt" },
			{ ColumnType::TimeUUID, "TimeUUID" },
			{ ColumnType::Inet, "Inet" },
			{ ColumnType::Date, "Date" },
			{ ColumnType::Time, "Time" },
			{ ColumnType::SmallInt, "SmallInt" },
			{ ColumnType::TinyInt, "TinyInt" },
			{ ColumnType::Duration, "Duration" },
			{ ColumnType::List, "List" },
			{ ColumnType::Map, "Map" },
			{ ColumnType::Set, "Set" },
			{ ColumnType::UDT, "UDT" },
			{ ColumnType::Tuple, "Tuple" },
		});
		return staticNames;
	}

	const std::vector<std::pair<ErrorCode, const char*>>&
		EnumDescriptions<ErrorCode>::get() {
		static std::vector<std::pair<ErrorCode, const char*>> staticNames({
			{ ErrorCode::ServerError, "ServerError" },
			{ ErrorCode::ProtocolError, "ProtocolError" },
			{ ErrorCode::AuthenticationError, "AuthenticationError" },
			{ ErrorCode::Unavailable, "Unavailable" },
			{ ErrorCode::Overloaded, "Overloaded" },
			{ ErrorCode::IsBootstrapping, "IsBootstrapping" },
			{ ErrorCode::TruncateError, "TruncateError" },
			{ ErrorCode::WriteTimeout, "WriteTimeout" },
			{ ErrorCode::ReadTimeout, "ReadTimeout" },
			{ ErrorCode::ReadFailure, "ReadFailure" },
			{ ErrorCode::FunctionFailure, "FunctionFailure" },
			{ ErrorCode::WriteFailure, "WriteFailure" },
			{ ErrorCode::SyntaxError, "SyntaxError" },
			{ ErrorCode::UnAuthorized, "UnAuthorized" },
			{ ErrorCode::InvalidQuery, "InvalidQuery" },
			{ ErrorCode::ConfigError, "ConfigError" },
			{ ErrorCode::AlreadyExists, "AlreadyExists" },
			{ ErrorCode::UnPreparedQuery, "UnPreparedQuery" },
		});
		return staticNames;
	}
}

