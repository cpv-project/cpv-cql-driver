#pragma once
#include <cstdint>
#include <core/sstring.hh>

namespace cql {
	/**
	 * The base class of variable integer bytes
	 *
	 * The most significant byte will contains the information about how many extra bytes need to be read.
	 * The number of extra bytes to read is encoded as 1 bits on the left side.
	 * For example, if we need to read 2 more bytes the first byte will start with 110,
	 * eg 256000 will be encoded on 3 bytes as [110]00011 11101000 00000000.
	 * If the encoded integer is 8 bytes long the vint will be encoded on 9 bytes and
	 * the first byte will be 11111111.
	 *
	 * Notice:
	 * Integer greater than 8 bytes is unsupported, check VIntCoding.java in cassandra repository.
	 */
	class ProtocolVariableIntegerBase {
	public:
		/** Get the integer value */
		std::uint64_t get() const { return value_; }

		/** Set the integer value */
		void set(std::uint64_t value) { value_ = value; }

		/** Reset to initial state */
		void reset() { value_ = 0; }

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructors */
		ProtocolVariableIntegerBase() : value_() { }
		explicit ProtocolVariableIntegerBase(std::uint64_t value) :
			value_(value) { }

	protected:
		std::uint64_t value_;
	};
}

