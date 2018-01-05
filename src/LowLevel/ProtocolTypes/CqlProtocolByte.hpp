#pragma once
#include <cstdint>

namespace cql {
	/** A 1 byte unsigned integer */
	class CqlProtocolByte {
	public:
		std::uint8_t get() const { return value_; }
		void set(std::uint8_t value) { value_ = value_; }
		CqlProtocolByte(std::uint8_t value) : value_(value) {}

	private:
		std::uint8_t value_;
	};
}

