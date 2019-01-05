#pragma once
#include "./ProtocolSizedStringBase.hpp"

namespace cql {
	/** [value] can be either not set, null or normal state */
	enum class ProtocolValueState { Normal = 0, Null = -1, NotSet = -2 };

	/**
	 * A [int] n, followed by n bytes if n >= 0,
	 * if n == -1, no byte should follow and the value represented is `null`.
	 * if n == -2, no byte should follow and the value represented is `not set`.
	 * n < -2 is an invalid value and results in an error.
	 */
	class ProtocolValue : private ProtocolSizedStringBase<
		std::int32_t,
		ProtocolValueState,
		ProtocolValueState::NotSet, // default state is not set
		ProtocolValueState::Normal> {
	public:
		using ProtocolSizedStringBase::get;
		using ProtocolSizedStringBase::set;
		using ProtocolSizedStringBase::append;
		using ProtocolSizedStringBase::reset;
		using ProtocolSizedStringBase::state;
		using ProtocolSizedStringBase::encode;
		using ProtocolSizedStringBase::ProtocolSizedStringBase;

		/** Check whether the value is representing null */
		bool isNull() const { return state_ == ProtocolValueState::Null; }

		/** Check whether the value is representing "not set" */
		bool isNotSet() const { return state_ == ProtocolValueState::NotSet; }

		/** Decode from binary data */
		void decode(const char*& ptr, const char* end) {
			ProtocolSizedStringBase::decode(ptr, end);
			if (CQL_UNLIKELY(
				state_ != ProtocolValueState::Normal &&
				state_ != ProtocolValueState::Null &&
				state_ != ProtocolValueState::NotSet)) {
				throw DecodeException(CQL_CODEINFO, "invalid negative string length");
			}
		}
	};
}

