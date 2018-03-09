#pragma once
#include "./IntegerBase.hpp"

namespace cql {
	/**
	 * A single byte.
	 * A value of 0 denotes "false"; any other value denotes "true".
	 * (However, it is recommended that a value of 1 be used to represent "true").
	 */
	class Boolean : private IntegerBase<std::int8_t> {
	public:
		using IntegerBase::UnderlyingType;
		using IntegerBase::EncodeSize;
		using IntegerBase::reset;
		using IntegerBase::encodeBody;
		using IntegerBase::decodeBody;
		using IntegerBase::operator UnderlyingType;
		using IntegerBase::operator bool;

		void set(bool value) { value_ = static_cast<UnderlyingType>(value); }
		bool get() const { return static_cast<bool>(value_); }
		Boolean() : IntegerBase(0) { }
		// cppcheck-suppress noExplicitConstructor
		Boolean(bool value) : IntegerBase(static_cast<UnderlyingType>(value)) { }
		void operator=(bool value) { value_ = static_cast<UnderlyingType>(value); }
	};
}

