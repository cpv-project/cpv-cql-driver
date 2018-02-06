#pragma once
#include "CqlIntegerBase.hpp"

namespace cql {
	/** A 2 byte two's complement integer */
	class CqlSmallInt : private CqlIntegerBase<std::int16_t> {
	public:
		using CqlIntegerBase::CqlUnderlyingType;
		using CqlIntegerBase::CqlEncodeSize;
		using CqlIntegerBase::get;
		using CqlIntegerBase::set;
		using CqlIntegerBase::reset;
		using CqlIntegerBase::encode;
		using CqlIntegerBase::decode;
		using CqlIntegerBase::CqlIntegerBase;
		using CqlIntegerBase::operator=;
		using CqlIntegerBase::operator CqlUnderlyingType;
		using CqlIntegerBase::operator bool;
	};
}

