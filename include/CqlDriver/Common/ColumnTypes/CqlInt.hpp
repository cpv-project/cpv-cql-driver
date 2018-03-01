#pragma once
#include "CqlIntegerBase.hpp"

namespace cql {
	/** A 4 byte two's complement integer */
	class CqlInt : private CqlIntegerBase<std::int32_t> {
	public:
		using CqlIntegerBase::CqlUnderlyingType;
		using CqlIntegerBase::CqlEncodeSize;
		using CqlIntegerBase::get;
		using CqlIntegerBase::set;
		using CqlIntegerBase::reset;
		using CqlIntegerBase::encodeBody;
		using CqlIntegerBase::decodeBody;
		using CqlIntegerBase::CqlIntegerBase;
		using CqlIntegerBase::operator=;
		using CqlIntegerBase::operator CqlUnderlyingType;
		using CqlIntegerBase::operator bool;
	};
}

