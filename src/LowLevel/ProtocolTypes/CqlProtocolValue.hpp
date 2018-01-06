#pragma once
#include <cstdint>
#include <core/sstring.hh>

namespace cql {
	/**
	 * A [int] n, followed by n bytes if n >= 0,
	 * if n == -1, no byte should follow and the value represented is `null`.
	 * if n == -2, no byte should follow and the value represented is `not set`.
	 * n < -2 is an invalid value and results in an error.
	 */
	class CqlProtocolValue {
	public:
		using LengthType = std::int32_t;
		enum class State : LengthType { NotSet = -2, Null = -1, Normal = 0 };

		const seastar::sstring& get() const& { return value_; }
		seastar::sstring& get() & { return value_; }
		State state() const { return state_; }
		bool isNull() const { return state_ == State::Null; }
		bool isNotSet() const { return state_ == State::NotSet; }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		explicit CqlProtocolValue(State state) : value_(), state_(state) { }

		explicit CqlProtocolValue(seastar::sstring&& value) :
			value_(std::move(value)), state_(State::Normal) {}

	private:
		seastar::sstring value_;
		State state_;
	};
}

