#pragma once
#include <chrono>
#include "./ProtocolIntegerBase.hpp"

namespace cql {
	/** A [long] representing the timestamp in microseconds (negative values are forbidden) */
	class ProtocolTimestamp : private ProtocolIntegerBase<std::int64_t> {
	public:
		using ProtocolIntegerBase::reset;
		using ProtocolIntegerBase::encode;
		using ProtocolIntegerBase::decode;

		/** Get the timestamp value */
		std::chrono::system_clock::time_point get() const;

		/** Set the timestamp value */
		void set(std::chrono::system_clock::time_point value);

		/** Constructors */
		ProtocolTimestamp();
		explicit ProtocolTimestamp(std::chrono::system_clock::time_point value);
	};
}

