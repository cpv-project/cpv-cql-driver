#pragma once
#include "./ProtocolSizedStringBase.hpp"

namespace cql {
	/** [bytes] can be either null or normal state */
	enum class ProtocolBytesState { Normal = 0, Null = -1 };

	/**
	 * A [int] n, followed by n bytes if n >= 0,
	 * if n < 0, no bytes should follow and the value represented is `null`.
	 */
	class ProtocolBytes : private ProtocolSizedStringBase<
		std::int32_t,
		ProtocolBytesState,
		ProtocolBytesState::Null, // default state is null
		ProtocolBytesState::Normal> {
	public:
		using ProtocolSizedStringBase::get;
		using ProtocolSizedStringBase::set;
		using ProtocolSizedStringBase::append;
		using ProtocolSizedStringBase::reset;
		using ProtocolSizedStringBase::encode;
		using ProtocolSizedStringBase::ProtocolSizedStringBase;

		/** Check whether the value is representing null */
		bool isNull() const { return state_ == ProtocolBytesState::Null; }

		/** Decode from binary data */
		void decode(const char*& ptr, const char* end) {
			ProtocolSizedStringBase::decode(ptr, end);
			if (state_ != ProtocolBytesState::Normal) {
				// the specification didn't said negative value other than -1 is invalid
				state_ = ProtocolBytesState::Null;
			}
		}
	};
}

