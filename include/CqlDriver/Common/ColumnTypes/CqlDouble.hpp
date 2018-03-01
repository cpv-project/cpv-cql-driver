#pragma once
#include "CqlFloatingPointBase.hpp"

namespace cql {
	/** A 8 byte floating point number in the IEEE 754 binary64 format */
	class CqlDouble : private CqlFloatingPointBase<double> {
	public:
		using CqlFloatingPointBase::CqlUnderlyingType;
		using CqlFloatingPointBase::CqlEncodeSize;
		using CqlFloatingPointBase::get;
		using CqlFloatingPointBase::set;
		using CqlFloatingPointBase::reset;
		using CqlFloatingPointBase::approximatelyEquals;
		using CqlFloatingPointBase::encodeBody;
		using CqlFloatingPointBase::decodeBody;
		using CqlFloatingPointBase::CqlFloatingPointBase;
		using CqlFloatingPointBase::operator=;
		using CqlFloatingPointBase::operator CqlUnderlyingType;

		static_assert(sizeof(CqlUnderlyingType) == 8, "sizeof(double) should be 8");
	};
}

