#pragma once
#include <CQLDriver/Common/Utility/EnumUtils.hpp>
#include <ostream>

namespace cql {
	// For more information, check the cql protocol definition on:
	// https://github.com/apache/cassandra/blob/trunk/doc/native_protocol_v4.spec

	/**
	 * Message direction, either request or response
	 * The value of this enum can use to calculate the <version> in frame header,
	 * for example: header.version = static_cast<std::size_t>(direction) | DefaultVerion;
	 */
	enum class MessageDirection {
		Request = 0,
		Response = 0x80,
		Mask_ = 0x80
	};

	template <>
	struct EnumDescriptions<MessageDirection> {
		static const std::vector<std::pair<MessageDirection, const char*>>& get();
	};

	/**
	 * Message type, each type is either request type or response type
	 * The value of this enum is the <opcode> in frame header.
	 * Check native_protocol_v4.spec section 2.4.
	 */
	enum class MessageType {
		Error = 0,
		Startup = 1,
		Ready = 2,
		Authenticate = 3,
		Options = 5,
		Supported = 6,
		Query = 7,
		Result = 8,
		Prepare = 9,
		Execute = 0xa,
		Register = 0xb,
		Event = 0xc,
		Batch = 0xd,
		AuthChallenge = 0xe,
		AuthResponse = 0xf,
		AuthSuccess = 0x10,
		Max_ = 0x11 // only for array definition
	};

	template <>
	struct EnumDescriptions<MessageType> {
		static const std::vector<std::pair<MessageType, const char*>>& get();
	};

	/**
	 * Flags in message header
	 * The value of this enum is the <flags> in frame header.
	 * Check native_protocol_v4.spec section 2.2.
	 */
	enum class MessageHeaderFlags {
		None = 0,
		Compression = 1,
		Tracing = 2,
		CustomPayload = 4,
		Warning = 8
	};

	template <>
	struct EnumDescriptions<MessageHeaderFlags> {
		static const std::vector<std::pair<MessageHeaderFlags, const char*>>& get();
	};

	/**
	 * Flags in query parameters.
	 * The value of this enum is the <flags> in query parameters.
	 * Check native_protocol_v4.spec section 4.1.4.
	 */
	enum class QueryParametersFlags {
		None = 0,
		WithValues = 1,
		SkipMetadata = 2,
		WithPageSize = 4,
		WithPagingState = 8,
		WithSerialConsistency = 16,
		WithDefaultTimestamp = 32,
		WithNamesForValue = 64
	};

	template <>
	struct EnumDescriptions<QueryParametersFlags> {
		static const std::vector<std::pair<QueryParametersFlags, const char*>>& get();
	};

	/**
	 * Flags in batch parameters.
	 * The value of this enum is the <flags> in batch parameters.
	 * Check native_protocol_v4.spec section 4.1.7.
	 */
	enum class BatchParametersFlags {
		None = 0,
		WithSerialConsistency = 16,
		WithDefaultTimestamp = 32
	};

	template <>
	struct EnumDescriptions<BatchParametersFlags> {
		static const std::vector<std::pair<BatchParametersFlags, const char*>>& get();
	};

	/**
	 * Indicating whether the following query is a prepared one or not
	 * The value of this enum is the <kind> in batch query.
	 * Check native_protocol_v4.spec section 4.1.7.
	 */
	enum class BatchQueryKind {
		Query = 0,
		PreparedQueryId = 1
	};

	template <>
	struct EnumDescriptions<BatchQueryKind> {
		static const std::vector<std::pair<BatchQueryKind, const char*>>& get();
	};

	/**
	 * The kind of the query result.
	 * The value of this enum is the <kind> in result message.
	 * Check native_protocol_v4.spec section 4.2.5.
	 */
	enum class ResultKind {
		Unknown = 0x0000,
		Void = 0x0001,
		Rows = 0x0002,
		SetKeySpace = 0x0003,
		Prepared = 0x0004,
		SchemaChange = 0x0005
	};

	template <>
	struct EnumDescriptions<ResultKind> {
		static const std::vector<std::pair<ResultKind, const char*>>& get();
	};

	/**
	 * Flags in metadata of rows result.
	 * The value of this enum is the <flags> in metadata of rows result.
	 * Check native_protocol_v4.spec section 4.2.5.2.
	 */
	enum class ResultRowsMetadataFlags {
		None = 0,
		GlobalTableSpec = 1,
		HasMorePages = 2,
		NoMetadata = 4
	};

	template <>
	struct EnumDescriptions<ResultRowsMetadataFlags> {
		static const std::vector<std::pair<ResultRowsMetadataFlags, const char*>>& get();
	};

	/**
	 * Flags in metadata of prepared result.
	 * The value of this enum is the <flags> in metadata of prepared result.
	 * Check native_protocol_v4.spec section 4.2.5.4.
	 */
	enum class ResultPreparedMetadataFlags {
		None = 0,
		GlobalTableSpec = 1
	};

	template <>
	struct EnumDescriptions<ResultPreparedMetadataFlags> {
		static const std::vector<std::pair<ResultPreparedMetadataFlags, const char*>>& get();
	};
}

