#include "CqlLowLevelDefinitions.hpp"
#include <array>
#include <vector>
#include <utility>

namespace cql {
	const std::vector<std::pair<CqlMessageDirection, const char*>>&
		EnumDescriptions<CqlMessageDirection>::get() {
		static std::vector<std::pair<CqlMessageDirection, const char*>> staticNames({
			{ CqlMessageDirection::Request, "Request" },
			{ CqlMessageDirection::Response, "Response" }
		});
		return staticNames;
	}

	const std::vector<std::pair<CqlMessageType, const char*>>&
		EnumDescriptions<CqlMessageType>::get() {
		static std::vector<std::pair<CqlMessageType, const char*>> staticNames({
			{ CqlMessageType::Error, "Error" },
			{ CqlMessageType::Startup, "Startup" },
			{ CqlMessageType::Ready, "Ready" },
			{ CqlMessageType::Authenticate, "Authenticate" },
			{ CqlMessageType::Options, "Options" },
			{ CqlMessageType::Supported, "Supported" },
			{ CqlMessageType::Query, "Query" },
			{ CqlMessageType::Result, "Result" },
			{ CqlMessageType::Prepare, "Prepare" },
			{ CqlMessageType::Execute, "Execute" },
			{ CqlMessageType::Register, "Register" },
			{ CqlMessageType::Event, "Event" },
			{ CqlMessageType::Batch, "Batch" },
			{ CqlMessageType::AuthChallenge, "AuthChallenge" },
			{ CqlMessageType::AuthResponse, "AuthResponse" },
			{ CqlMessageType::AuthSuccess, "AuthSuccess" },
		});
		return staticNames;
	}

	const std::vector<std::pair<CqlMessageHeaderFlags, const char*>>&
		EnumDescriptions<CqlMessageHeaderFlags>::get() {
		static std::vector<std::pair<CqlMessageHeaderFlags, const char*>> staticNames({
			{ CqlMessageHeaderFlags::None, "None" },
			{ CqlMessageHeaderFlags::Compression, "Compression" },
			{ CqlMessageHeaderFlags::Tracing, "Tracing" },
			{ CqlMessageHeaderFlags::CustomPayload, "CustomPayload" },
			{ CqlMessageHeaderFlags::Warning, "Warning" }
		});
		return staticNames;
	}

	const std::vector<std::pair<CqlQueryParametersFlags, const char*>>&
		EnumDescriptions<CqlQueryParametersFlags>::get() {
		static std::vector<std::pair<CqlQueryParametersFlags, const char*>> staticNames({
			{ CqlQueryParametersFlags::None, "None" },
			{ CqlQueryParametersFlags::WithValues, "WithValues" },
			{ CqlQueryParametersFlags::SkipMetadata, "SkipMetadata" },
			{ CqlQueryParametersFlags::WithPageSize, "WithPageSize" },
			{ CqlQueryParametersFlags::WithPagingState, "WithPagingState" },
			{ CqlQueryParametersFlags::WithSerialConsistency, "WithSerialConsistency" },
			{ CqlQueryParametersFlags::WithDefaultTimestamp, "WithDefaultTimestamp" },
			{ CqlQueryParametersFlags::WithNamesForValue, "WithNamesForValue" }
		});
		return staticNames;
	}

	const std::vector<std::pair<CqlBatchParametersFlags, const char*>>&
		EnumDescriptions<CqlBatchParametersFlags>::get() {
		static std::vector<std::pair<CqlBatchParametersFlags, const char*>> staticNames({
			{ CqlBatchParametersFlags::None, "None" },
			{ CqlBatchParametersFlags::WithSerialConsistency, "WithSerialConsistency" },
			{ CqlBatchParametersFlags::WithDefaultTimestamp, "WithDefaultTimestamp" }
		});
		return staticNames;
	}

	const std::vector<std::pair<CqlBatchQueryKind, const char*>>&
		EnumDescriptions<CqlBatchQueryKind>::get() {
		static std::vector<std::pair<CqlBatchQueryKind, const char*>> staticNames({
			{ CqlBatchQueryKind::Query, "Query" },
			{ CqlBatchQueryKind::PreparedQueryId, "PreparedQueryId" },
		});
		return staticNames;
	}
}

