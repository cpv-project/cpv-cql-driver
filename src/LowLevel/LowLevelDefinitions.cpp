#include <array>
#include <vector>
#include <utility>
#include "./LowLevelDefinitions.hpp"

namespace cql {
	const std::vector<std::pair<MessageDirection, const char*>>&
		EnumDescriptions<MessageDirection>::get() {
		static std::vector<std::pair<MessageDirection, const char*>> staticNames({
			{ MessageDirection::Request, "Request" },
			{ MessageDirection::Response, "Response" }
		});
		return staticNames;
	}

	const std::vector<std::pair<MessageType, const char*>>&
		EnumDescriptions<MessageType>::get() {
		static std::vector<std::pair<MessageType, const char*>> staticNames({
			{ MessageType::Error, "Error" },
			{ MessageType::Startup, "Startup" },
			{ MessageType::Ready, "Ready" },
			{ MessageType::Authenticate, "Authenticate" },
			{ MessageType::Options, "Options" },
			{ MessageType::Supported, "Supported" },
			{ MessageType::Query, "Query" },
			{ MessageType::Result, "Result" },
			{ MessageType::Prepare, "Prepare" },
			{ MessageType::Execute, "Execute" },
			{ MessageType::Register, "Register" },
			{ MessageType::Event, "Event" },
			{ MessageType::Batch, "Batch" },
			{ MessageType::AuthChallenge, "AuthChallenge" },
			{ MessageType::AuthResponse, "AuthResponse" },
			{ MessageType::AuthSuccess, "AuthSuccess" },
		});
		return staticNames;
	}

	const std::vector<std::pair<MessageHeaderFlags, const char*>>&
		EnumDescriptions<MessageHeaderFlags>::get() {
		static std::vector<std::pair<MessageHeaderFlags, const char*>> staticNames({
			{ MessageHeaderFlags::None, "None" },
			{ MessageHeaderFlags::Compression, "Compression" },
			{ MessageHeaderFlags::Tracing, "Tracing" },
			{ MessageHeaderFlags::CustomPayload, "CustomPayload" },
			{ MessageHeaderFlags::Warning, "Warning" }
		});
		return staticNames;
	}

	const std::vector<std::pair<QueryParametersFlags, const char*>>&
		EnumDescriptions<QueryParametersFlags>::get() {
		static std::vector<std::pair<QueryParametersFlags, const char*>> staticNames({
			{ QueryParametersFlags::None, "None" },
			{ QueryParametersFlags::WithValues, "WithValues" },
			{ QueryParametersFlags::SkipMetadata, "SkipMetadata" },
			{ QueryParametersFlags::WithPageSize, "WithPageSize" },
			{ QueryParametersFlags::WithPagingState, "WithPagingState" },
			{ QueryParametersFlags::WithSerialConsistency, "WithSerialConsistency" },
			{ QueryParametersFlags::WithDefaultTimestamp, "WithDefaultTimestamp" },
			{ QueryParametersFlags::WithNamesForValue, "WithNamesForValue" }
		});
		return staticNames;
	}

	const std::vector<std::pair<BatchParametersFlags, const char*>>&
		EnumDescriptions<BatchParametersFlags>::get() {
		static std::vector<std::pair<BatchParametersFlags, const char*>> staticNames({
			{ BatchParametersFlags::None, "None" },
			{ BatchParametersFlags::WithSerialConsistency, "WithSerialConsistency" },
			{ BatchParametersFlags::WithDefaultTimestamp, "WithDefaultTimestamp" }
		});
		return staticNames;
	}

	const std::vector<std::pair<BatchQueryKind, const char*>>&
		EnumDescriptions<BatchQueryKind>::get() {
		static std::vector<std::pair<BatchQueryKind, const char*>> staticNames({
			{ BatchQueryKind::Query, "Query" },
			{ BatchQueryKind::PreparedQueryId, "PreparedQueryId" },
		});
		return staticNames;
	}

	const std::vector<std::pair<BatchType, const char*>>&
		EnumDescriptions<BatchType>::get() {
		static std::vector<std::pair<BatchType, const char*>> staticNames({
			{ BatchType::Logged, "Logged" },
			{ BatchType::UnLogged, "UnLogged" },
			{ BatchType::Counter, "Counter" },
		});
		return staticNames;
	}

	const std::vector<std::pair<ResultKind, const char*>>&
		EnumDescriptions<ResultKind>::get() {
		static std::vector<std::pair<ResultKind, const char*>> staticNames({
			{ ResultKind::Unknown, "Unknown" },
			{ ResultKind::Void, "Void" },
			{ ResultKind::Rows, "Rows" },
			{ ResultKind::SetKeySpace, "SetKeySpace" },
			{ ResultKind::Prepared, "Prepared" },
			{ ResultKind::SchemaChange, "SchemaChange" },
		});
		return staticNames;
	}

	const std::vector<std::pair<ResultRowsMetadataFlags, const char*>>&
		EnumDescriptions<ResultRowsMetadataFlags>::get() {
		static std::vector<std::pair<ResultRowsMetadataFlags, const char*>> staticNames({
			{ ResultRowsMetadataFlags::None, "None" },
			{ ResultRowsMetadataFlags::GlobalTableSpec, "GlobalTableSpec" },
			{ ResultRowsMetadataFlags::HasMorePages, "HasMorePages" },
			{ ResultRowsMetadataFlags::NoMetadata, "NoMetadata" },
		});
		return staticNames;
	}

	const std::vector<std::pair<ResultPreparedMetadataFlags, const char*>>&
		EnumDescriptions<ResultPreparedMetadataFlags>::get() {
		static std::vector<std::pair<ResultPreparedMetadataFlags, const char*>> staticNames({
			{ ResultPreparedMetadataFlags::None, "None" },
			{ ResultPreparedMetadataFlags::GlobalTableSpec, "GlobalTableSpec" },
		});
		return staticNames;
	}
}

