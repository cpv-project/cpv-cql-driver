#pragma once
#include <cstdint>
#include <core/sstring.hh>

namespace cql {
	/**
	 * A [short] n, followed by n bytes representing an UTF-8 string
	 */
	class CqlProtocolString {
	public:
		using LengthType = std::uint16_t;

		const seastar::sstring& get() const& { return value_; }
		seastar::sstring& get() & { return value_; }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		explicit CqlProtocolString(seastar::sstring&& value) : value_(std::move(value)) {}

	private:
		seastar::sstring value_;
	};
}

