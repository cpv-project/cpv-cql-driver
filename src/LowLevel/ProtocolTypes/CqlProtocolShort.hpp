#pragma once
#include <cstdint>

namespace cql {
	/** A 2 bytes unsigned integer */
	class CqlProtocolShort {
	public:
		std::uint16_t get() const { return value_; }
		void set(std::uint16_t value) { value_ = value_; }
		CqlProtocolShort(std::uint16_t value) : value_(value) {}

	private:
		std::uint16_t value_;
	};
}

