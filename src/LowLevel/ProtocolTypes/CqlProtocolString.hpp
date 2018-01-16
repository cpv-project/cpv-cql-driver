#pragma once
#include "CqlProtocolSizedStringBase.hpp"

namespace cql {
	/** [string] can't be null or not set, only normal state */
	enum class CqlProtocolStringState { Normal = 0 };

	/**
	 * A [short] n, followed by n bytes representing an UTF-8 string
	 */
	class CqlProtocolString : private CqlProtocolSizedStringBase<
		std::uint16_t,
		CqlProtocolStringState,
		CqlProtocolStringState::Normal,
		CqlProtocolStringState::Normal> {
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

