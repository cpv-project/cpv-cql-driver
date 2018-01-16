#pragma once
#include <CqlDriver/Common/Utility/EnumUtils.hpp>
#include <ostream>

namespace cql {
	// For more information, check the cql protocol definition on:
	// https://github.com/apache/cassandra/blob/trunk/doc/native_protocol_v5.spec

	/**
	 * Message direction, either request or response
	 * The value of this enum can use to calculate the `version` in frame header,
	 * for example: header.version = static_cast<std::size_t>(direction) | DefaultCqlVerion;
	 */
	enum class CqlMessageDirection {
		Request = 0,
		Response = 0x80,
		Mask_ = 0x80
	};

	/**
	 * Message type, each type is either request type or response type
	 * The value of this enum is the `opcode` in frame header.
	 * Check native_protocol_v4.spec section 2.4.
	 */
	enum class CqlMessageType {
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

	/** Write the text description of message type to stream */
	std::ostream& operator<<(std::ostream& stream, CqlMessageType type);

	/**
	 * Flags in message header
	 * The value of this enum is the `flags` in frame header.
	 * Check native_protocol_v4.spec section 2.2.
	 */
	enum class CqlMessageHeaderFlags {
		None = 0,
		Compression = 1,
		Tracing = 2,
		CustomPayload = 4,
		Warning = 8,
		UseBeta = 16
	};

	/** Write the text description of header flags to stream */
	std::ostream& operator<<(std::ostream& stream, CqlMessageHeaderFlags flags);

	/**
	 * Flags in query parameters.
	 * The value of this enum is the `flags` in query parameters.
	 * Check native_protocol_v4.spec section 4.1.4.
	 */
	enum CqlQueryParametersFlags {
		None = 0,
		WithValues = 1,
		SkipMetadata = 2,
		WithPageSize = 4,
		WithPagingState = 8,
		WithSerialConsistency = 16,
		WithDefaultTimestamp = 32,
		WithNamesForValue = 64,
		WithKeySpace = 128
	};

	/** Write the text description of query parameters flags to stream */
	std::ostream& operator<<(std::ostream& stream, CqlQueryParametersFlags flags);
}

