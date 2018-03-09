#pragma once
#include "./IntegerBase.hpp"

namespace cql {
	/** A 8 byte two's complement integer */
	class BigInt : private IntegerBase<std::int64_t> {
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

