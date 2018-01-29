#pragma once
#include "CqlIntegerBase.hpp"

namespace cql {
	/** Same as "bigint", but able to perform increment and decrement */
	class CqlCounter : private CqlIntegerBase<std::int64_t> {
	public:
		using CqlIntegerBase::CqlUnderlyingType;
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

