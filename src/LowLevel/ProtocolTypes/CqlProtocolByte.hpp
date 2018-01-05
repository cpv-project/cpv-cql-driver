#pragma once
#include <cstdint>
#include <core/sstring.hh>

namespace cql {
	/** A 1 byte unsigned integer */
	class CqlProtocolByte {
	public:
		std::uint8_t get() const { return value_; }
		void set(std::uint8_t value) { value_ = value; }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		explicit CqlProtocolByte(std::uint8_t value) : value_(value) {}

	private:
		std::uint8_t value_;
	};
}

