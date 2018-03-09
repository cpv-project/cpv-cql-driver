#pragma once
#include "./ProtocolIntegerBase.hpp"

namespace cql {
	/** A 2 bytes unsigned integer */
	class ProtocolShort : private ProtocolIntegerBase<std::uint16_t> {
	public:
		using ProtocolIntegerBase::get;
		using ProtocolIntegerBase::set;
		using ProtocolIntegerBase::reset;
		using ProtocolIntegerBase::encode;
		using ProtocolIntegerBase::decode;
		using ProtocolIntegerBase::ProtocolIntegerBase;
	};
}

