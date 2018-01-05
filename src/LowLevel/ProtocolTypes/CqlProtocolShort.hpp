#pragma once
#include <cstdint>
#include <core/sstring.hh>

namespace cql {
	/** A 2 bytes unsigned integer */
	class CqlProtocolShort {
	public:
		std::uint16_t get() const { return value_; }
		void set(std::uint16_t value) { value_ = value; }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		explicit CqlProtocolShort(std::uint16_t value) : value_(value) {}

	private:
		std::uint16_t value_;
	};
}

