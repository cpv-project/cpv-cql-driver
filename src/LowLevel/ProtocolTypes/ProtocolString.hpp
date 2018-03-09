#pragma once
#include "./ProtocolSizedStringBase.hpp"

namespace cql {
	/** [string] can't be null or not set, only normal state */
	enum class ProtocolStringState { Normal = 0 };

	/**
	 * A [short] n, followed by n bytes representing an UTF-8 string
	 */
	class ProtocolString : private ProtocolSizedStringBase<
		std::uint16_t,
		ProtocolStringState,
		ProtocolStringState::Normal,
		ProtocolStringState::Normal> {
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

