#pragma once
#include "./ProtocolSizedStringBase.hpp"

namespace cql {
	/** [short bytes] can't be null or not set, only normal state */
	enum class ProtocolShortBytesState { Normal = 0 };

	/**
	 * A [short] n, followed by n bytes if n >= 0,
	 * different to ProtocolBytes, ProtocolShortBytes can't represent `null`
	 */
	class ProtocolShortBytes : private ProtocolSizedStringBase<
		std::uint16_t,
		ProtocolShortBytesState,
		ProtocolShortBytesState::Normal,
		ProtocolShortBytesState::Normal> {
	public:
		using ProtocolSizedStringBase::get;
		using ProtocolSizedStringBase::set;
		using ProtocolSizedStringBase::append;
		using ProtocolSizedStringBase::reset;
		using ProtocolSizedStringBase::encode;
		using ProtocolSizedStringBase::decode;
		using ProtocolSizedStringBase::ProtocolSizedStringBase;
	};
}

