#pragma once
#include "./IntegerBase.hpp"

namespace cql {
	/** A 2 byte two's complement integer */
	class SmallInt : private IntegerBase<std::int16_t> {
	public:
		using IntegerBase::UnderlyingType;
		using IntegerBase::EncodeSize;
		using IntegerBase::get;
		using IntegerBase::set;
		using IntegerBase::reset;
		using IntegerBase::encodeBody;
		using IntegerBase::decodeBody;
		using IntegerBase::IntegerBase;
		using IntegerBase::operator=;
		using IntegerBase::operator UnderlyingType;
		using IntegerBase::operator bool;
	};
}

