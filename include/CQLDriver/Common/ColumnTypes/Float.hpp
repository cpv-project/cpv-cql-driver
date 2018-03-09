#pragma once
#include "./FloatingPointBase.hpp"

namespace cql {
	/** A 4 byte floating point number in the IEEE 754 binary32 format */
	class Float : private FloatingPointBase<float> {
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

		static_assert(sizeof(UnderlyingType) == 4, "sizeof(float) should be 4");
	};
}

