#pragma once
#include <cstdint>
#include <utility>
#include <core/sstring.hh>

namespace cql {
	/**
	 * A 16 bytes long uuid
	 * Example:
	 * TODO
	 */
	class CqlProtocolUuid {
	public:
		std::pair<std::uint64_t, std::uint64_t> get() const;
		void set(std::pair<std::uint64_t, std::uint64_t> value);

		seastar::sstring str() const;
		void set(const seastar::sstring& str);

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		static const CqlProtocolUuid& getEmpty();
		static CqlProtocolUuid makeRandom();

		explicit CqlProtocolUuid(const seastar::sstring& value);
		explicit CqlProtocolUuid(std::pair<std::uint64_t, std::uint64_t> value);
		explicit CqlProtocolUuid(std::uint64_t highBits, std::uint64_t lowBits);

	protected:
		std::uint64_t highBits_;
		std::uint64_t lowBits_;
	};
}

