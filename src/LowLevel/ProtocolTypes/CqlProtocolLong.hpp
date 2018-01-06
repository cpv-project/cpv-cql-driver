#pragma once
#include <cstdint>
#include <core/sstring.hh>

namespace cql {
	/** A 8 bytes signed integer */
	class CqlProtocolLong {
	public:
		std::int64_t get() const { return value_; }
		void set(std::int64_t value) { value_ = value; }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		explicit CqlProtocolLong(std::int64_t value) : value_(value) {}

	private:
		std::int64_t value_;
	};
}

