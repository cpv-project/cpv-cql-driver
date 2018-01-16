#pragma once
#include "CqlProtocolSizedStringBase.hpp"

namespace cql {
	/** [value] can be either not set, null or normal state */
	enum class CqlProtocolValueState { Normal = 0, Null = -1, NotSet = -2 };

	/**
	 * A [int] n, followed by n bytes if n >= 0,
	 * if n == -1, no byte should follow and the value represented is `null`.
	 * if n == -2, no byte should follow and the value represented is `not set`.
	 * n < -2 is an invalid value and results in an error.
	 */
	class CqlProtocolValue : private CqlProtocolSizedStringBase<
		std::int32_t,
		CqlProtocolValueState,
		CqlProtocolValueState::NotSet, // default state is not set
		CqlProtocolValueState::Normal> {
	public:
		using CqlProtocolSizedStringBase::get;
		using CqlProtocolSizedStringBase::set;
		using CqlProtocolSizedStringBase::append;
		using CqlProtocolSizedStringBase::reset;
		using CqlProtocolSizedStringBase::state;
		using CqlProtocolSizedStringBase::encode;
		using CqlProtocolSizedStringBase::CqlProtocolSizedStringBase;

		bool isNull() const { return state_ == CqlProtocolValueState::Null; }
		bool isNotSet() const { return state_ == CqlProtocolValueState::NotSet; }
		void decode(const char*& ptr, const char* end) {
			CqlProtocolSizedStringBase::decode(ptr, end);
			if (state_ != CqlProtocolValueState::Normal &&
				state_ != CqlProtocolValueState::Null &&
				state_ != CqlProtocolValueState::NotSet) {
				throw CqlDecodeException(CQL_CODEINFO, "invalid negative string length");
			}
		}
	};
}

