#pragma once
#include "CqlProtocolSizedStringBase.hpp"

namespace cql {
	/** [short bytes] can't be null or not set, only normal state */
	enum class CqlProtocolShortBytesState { Normal = 0 };

	/**
	 * A [short] n, followed by n bytes if n >= 0,
	 * different to CqlProtocolBytes, CqlProtocolShortBytes can't represent `null`
	 */
	class CqlProtocolShortBytes : private CqlProtocolSizedStringBase<
		std::uint16_t,
		CqlProtocolShortBytesState,
		CqlProtocolShortBytesState::Normal,
		CqlProtocolShortBytesState::Normal> {
	public:
		using CqlProtocolSizedStringBase::get;
		using CqlProtocolSizedStringBase::set;
		using CqlProtocolSizedStringBase::append;
		using CqlProtocolSizedStringBase::reset;
		using CqlProtocolSizedStringBase::encode;
		using CqlProtocolSizedStringBase::decode;
		using CqlProtocolSizedStringBase::CqlProtocolSizedStringBase;
	};
}

