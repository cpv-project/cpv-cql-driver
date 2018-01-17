#pragma once
#include <cstdint>
#include <utility>
#include <core/sstring.hh>

namespace cql {
	/**
	 * A 16 bytes long uuid
	 * Example:
	 * CqlProtocolUuid uuid("00112233-4455-6677-8899-aabbccddeeff");
	 */
	class CqlProtocolUuid {
	public:
		/** Get the binary value of uuid */
		std::pair<std::uint64_t, std::uint64_t> get() const;

		/** Set the binary value of uuid */
		void set(std::pair<std::uint64_t, std::uint64_t> value);

		/** Reset to initial state */
		void reset();

		/** Get the string representation of uuid */
		seastar::sstring str() const;

		/** Set the uuid by it's string representation */
		void set(const seastar::sstring& str);

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Get the empty uuid */
		static const CqlProtocolUuid& getEmpty();

		/** Make a random uuid */
		static CqlProtocolUuid makeRandom();

		/** Constructors */
		explicit CqlProtocolUuid(const seastar::sstring& value);
		explicit CqlProtocolUuid(std::pair<std::uint64_t, std::uint64_t> value);
		explicit CqlProtocolUuid(std::uint64_t highBits, std::uint64_t lowBits);

	private:
		std::uint64_t highBits_;
		std::uint64_t lowBits_;
	};
}

