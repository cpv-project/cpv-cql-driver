#pragma once
#include "CqlProtocolSizedStringBase.hpp"

namespace cql {
	/** [string] can't be null or not set, only normal state */
	enum class CqlProtocolLongStringState { Normal = 0 };

	/**
	 * A [int] n, followed by n bytes representing an UTF-8 string
	 */
	class CqlProtocolLongString : protected CqlProtocolSizedStringBase<
		std::int32_t,
		CqlProtocolLongStringState,
		CqlProtocolLongStringState::Normal,
		CqlProtocolLongStringState::Normal> {
	public:
		using CqlProtocolSizedStringBase::get;
		using CqlProtocolSizedStringBase::encode;
		using CqlProtocolSizedStringBase::CqlProtocolSizedStringBase;

		void decode(const char*& ptr, const char* end) {
			CqlProtocolSizedStringBase::decode(ptr, end);
			if (state_ != CqlProtocolLongStringState::Normal) {
				throw CqlDecodeException(CQL_CODEINFO, "string length is negative");
			}
		}
	};
}

