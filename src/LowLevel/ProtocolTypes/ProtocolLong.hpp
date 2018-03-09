#pragma once
#include "./ProtocolIntegerBase.hpp"

namespace cql {
	/** A 8 bytes signed integer */
	class ProtocolLong : private ProtocolIntegerBase<std::int64_t> {
	public:
		using ProtocolIntegerBase::get;
		using ProtocolIntegerBase::set;
		using ProtocolIntegerBase::reset;
		using ProtocolIntegerBase::encode;
		using ProtocolIntegerBase::decode;
		using ProtocolIntegerBase::ProtocolIntegerBase;
	};
}

