#pragma once
#include <cstdint>

namespace cql {
	/** A 4 bytes signed integer */
	class CqlProtocolInt {
	public:
		std::int32_t get() const { return value_; }
		void set(std::int32_t value) { value_ = value_; }
		CqlProtocolInt(std::int32_t value) : value_(value) {}

	private:
		std::int32_t value_;
	};
}

