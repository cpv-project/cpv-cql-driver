#pragma once
#include "CqlProtocolVariableIntegerBase.hpp"

namespace cql {
	/**
	 * A signed variable length integer
	 * This is encoded using zig-zag encoding and then sent like an [unsigned int]
	 * For example: 0 = 0, -1 = 1, 1 = 2, -2 = 3, 2 = 4, -3 = 5, 3 = 6 and so forth
	 */
	class CqlProtocolVint : private CqlProtocolVariableIntegerBase {
	public:
		using CqlProtocolVariableIntegerBase::reset;
		using CqlProtocolVariableIntegerBase::encode;
		using CqlProtocolVariableIntegerBase::decode;

		/** Get the integer value */
		std::int64_t get() const {
			return static_cast<std::int64_t>((value_ >> 1) ^ -(value_ & 1));
		}

		/** Set the integer value */
		void set(std::int64_t value) {
			// avoid signed shifting
			value_ = (value < 0 ? 0xffff'ffff'ffff'ffffU : 0) ^ (static_cast<std::uint64_t>(value) << 1);
		}

		/** Constructors */
		CqlProtocolVint() {}
		explicit CqlProtocolVint(std::int64_t value) { CqlProtocolVint::set(value); }
	};
}

