#pragma once
#include "./ProtocolIntegerBase.hpp"

namespace cql {
	/** A 1 byte unsigned integer */
	class ProtocolByte: private ProtocolIntegerBase<std::uint8_t> {
	public:
		using ProtocolIntegerBase::get;
		using ProtocolIntegerBase::set;
		using ProtocolIntegerBase::reset;
		using ProtocolIntegerBase::encode;
		using ProtocolIntegerBase::decode;
		using ProtocolIntegerBase::ProtocolIntegerBase;
	};
}

