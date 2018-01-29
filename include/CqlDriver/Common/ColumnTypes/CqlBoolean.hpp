#pragma once
#include "CqlIntegerBase.hpp"

namespace cql {
	/**
	 * A single byte.
	 * A value of 0 denotes "false"; any other value denotes "true".
	 * (However, it is recommended that a value of 1 be used to represent "true").
	 */
	class CqlBoolean : private CqlIntegerBase<std::int8_t> {
	public:
		using CqlIntegerBase::CqlUnderlyingType;
		using CqlIntegerBase::reset;
		using CqlIntegerBase::encode;
		using CqlIntegerBase::decode;
		using CqlIntegerBase::operator CqlUnderlyingType;
		using CqlIntegerBase::operator bool;
		void set(bool value) { value_ = static_cast<CqlUnderlyingType>(value); }
		bool get() const { return static_cast<bool>(value_); }
		CqlBoolean() : CqlIntegerBase(0) { }
		CqlBoolean(bool value) : CqlIntegerBase(static_cast<CqlUnderlyingType>(value)) { }
		void operator=(bool value) { value_ = static_cast<CqlUnderlyingType>(value); }
	};
}

