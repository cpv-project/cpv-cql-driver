#pragma once
#include <cstdint>
#include <core/sstring.hh>

namespace cql {
	/** A 4 bytes signed integer */
	class CqlProtocolInt {
	public:
		std::int32_t get() const { return value_; }
		void set(std::int32_t value) { value_ = value; }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		explicit CqlProtocolInt(std::int32_t value) : value_(value) {}

	private:
		std::int32_t value_;
	};
}

