#pragma once
#include <chrono>
#include "CqlProtocolIntegerBase.hpp"

namespace cql {
	/** A [long] representing the timestamp in microseconds (negative values are forbidden) */
	class CqlProtocolTimeStamp : private CqlProtocolIntegerBase<std::int64_t> {
	public:
		using CqlProtocolIntegerBase::reset;
		using CqlProtocolIntegerBase::encode;
		using CqlProtocolIntegerBase::decode;

		/** Get the timestamp value */
		std::chrono::system_clock::time_point get() const;

		/** Set the timestamp value */
		void set(std::chrono::system_clock::time_point value);

		/** Constructors */
		CqlProtocolTimeStamp();
		explicit CqlProtocolTimeStamp(std::chrono::system_clock::time_point value);
	};
}

