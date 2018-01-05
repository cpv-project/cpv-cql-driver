#pragma once
#include <cstdint>
#include <core/sstring.hh>

namespace cql {
	/**
	 * A [int] n, followed by n bytes if n >= 0,
	 * if n < 0, no bytes should follow and the value represented is `null`.
	 */
	class CqlProtocolBytes {
	public:
		const seastar::sstring& get() const& { return value_; }
		seastar::sstring& get() & { return value_; }
		bool isNull() const { return isNull_; }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Construct null bytes */
		CqlProtocolBytes() : value_(), isNull_(true) { }

		/** Construct non-null bytes */
		CqlProtocolBytes(seastar::sstring&& value) :
			value_(std::move(value)), isNull_(false) {}

	private:
		seastar::sstring value_;
		bool isNull_;
	};
}

