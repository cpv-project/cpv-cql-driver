#pragma once
#include "./FloatingPointBase.hpp"

namespace cql {
	/** A 8 byte floating point number in the IEEE 754 binary64 format */
	class Double : private FloatingPointBase<double> {
	public:
		using FloatingPointBase::UnderlyingType;
		using FloatingPointBase::EncodeSize;
		using FloatingPointBase::get;
		using FloatingPointBase::set;
		using FloatingPointBase::reset;
		using FloatingPointBase::approximatelyEquals;
		using FloatingPointBase::encodeBody;
		using FloatingPointBase::decodeBody;
		using FloatingPointBase::FloatingPointBase;
		using FloatingPointBase::operator=;
		using FloatingPointBase::operator UnderlyingType;

		static_assert(sizeof(UnderlyingType) == 8, "sizeof(double) should be 8");
	};
}

