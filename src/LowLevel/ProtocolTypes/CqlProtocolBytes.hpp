#pragma once
#include "CqlProtocolSizedStringBase.hpp"

namespace cql {
	/** [bytes] can be either null or normal state */
	enum class CqlProtocolBytesState { Normal = 0, Null = -1 };

	/**
	 * A [int] n, followed by n bytes if n >= 0,
	 * if n < 0, no bytes should follow and the value represented is `null`.
	 */
	class CqlProtocolBytes : private CqlProtocolSizedStringBase<
		std::int32_t,
		CqlProtocolBytesState,
		CqlProtocolBytesState::Null, // default state is null
		CqlProtocolBytesState::Normal> {
	public:
		using CqlProtocolSizedStringBase::get;
		using CqlProtocolSizedStringBase::set;
		using CqlProtocolSizedStringBase::append;
		using CqlProtocolSizedStringBase::reset;
		using CqlProtocolSizedStringBase::encode;
		using CqlProtocolSizedStringBase::CqlProtocolSizedStringBase;

		/** Check whether the value is representing null */
		bool isNull() const { return state_ == CqlProtocolBytesState::Null; }

		/** Decode from binary data */
		void decode(const char*& ptr, const char* end) {
			CqlProtocolSizedStringBase::decode(ptr, end);
			if (state_ != CqlProtocolBytesState::Normal) {
				// the specification didn't said negative value other than -1 is invalid
				state_ = CqlProtocolBytesState::Null;
			}
		}
	};
}

