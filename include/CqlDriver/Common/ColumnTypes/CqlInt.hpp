#pragma once
#include "CqlIntegerBase.hpp"

namespace cql {
	/** A 4 byte two's complement integer */
	class CqlInt : private CqlIntegerBase<std::int32_t> {
	public:
		using CqlIntegerBase::CqlUnderlyingType;
		using CqlIntegerBase::get;
		using CqlIntegerBase::set;
		using CqlIntegerBase::reset;
		using CqlIntegerBase::encode;
		using CqlIntegerBase::decode;
		using CqlIntegerBase::CqlIntegerBase;
		using CqlIntegerBase::operator=;
		using CqlIntegerBase::operator std::int32_t;
		using CqlIntegerBase::operator bool;
	};
}

