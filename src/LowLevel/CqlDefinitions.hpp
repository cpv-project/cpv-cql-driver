#pragma once

namespace cql {
	/**
	 * Message direction, either request or response
	 * The value of this enum can use to calculate the `version` in frame header,
	 * for example: header.version = static_cast<std::size_t>(direction) | DefaultCqlVerion;
	 */
	enum class CqlMessageDirection {
		Request = 0,
		Response = 0x80
	};

	/**
	 * Message type, each type is either request type or response type
	 * The value of this enum is the `opcode` in frame header.
	 */
	enum class CqlMessageType {
		Error = 0,
		StartUp = 1,
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
}

