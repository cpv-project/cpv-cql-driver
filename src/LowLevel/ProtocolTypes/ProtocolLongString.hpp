#pragma once
#include "./ProtocolSizedStringBase.hpp"

namespace cql {
	/** [string] can't be null or not set, only normal state */
	enum class ProtocolLongStringState { Normal = 0 };

	/**
	 * A [int] n, followed by n bytes representing an UTF-8 string
	 */
	class ProtocolLongString : private ProtocolSizedStringBase<
		std::int32_t,
		ProtocolLongStringState,
		ProtocolLongStringState::Normal,
		ProtocolLongStringState::Normal> {
	public:
		using ProtocolSizedStringBase::get;
		using ProtocolSizedStringBase::set;
		using ProtocolSizedStringBase::reset;
		using ProtocolSizedStringBase::append;
		using ProtocolSizedStringBase::encode;
		using ProtocolSizedStringBase::ProtocolSizedStringBase;

		/** Decode from binary data */
		void decode(const char*& ptr, const char* end) {
			ProtocolSizedStringBase::decode(ptr, end);
			if (state_ != ProtocolLongStringState::Normal) {
				throw DecodeException(CQL_CODEINFO, "string length is negative");
			}
		}
	};
}

