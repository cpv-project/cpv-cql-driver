#pragma once
#include "CqlFloatingPointBase.hpp"

namespace cql {
	/** A 4 byte floating point number in the IEEE 754 binary32 format */
	class CqlFloat : private CqlFloatingPointBase<float> {
	public:
		using CqlFloatingPointBase::CqlUnderlyingType;
		using CqlFloatingPointBase::get;
		using CqlFloatingPointBase::set;
		using CqlFloatingPointBase::reset;
		using CqlFloatingPointBase::approximatelyEquals;
		using CqlFloatingPointBase::encode;
		using CqlFloatingPointBase::decode;
		using CqlFloatingPointBase::CqlFloatingPointBase;
		using CqlFloatingPointBase::operator=;
		using CqlFloatingPointBase::operator CqlUnderlyingType;

		static_assert(sizeof(CqlUnderlyingType) == 4, "sizeof(float) should be 4");
	};
}

