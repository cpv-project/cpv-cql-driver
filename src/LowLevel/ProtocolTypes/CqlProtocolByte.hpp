#pragma once
#include "CqlProtocolIntegerBase.hpp"

namespace cql {
	/** A 1 byte unsigned integer */
	class CqlProtocolByte: protected CqlProtocolIntegerBase<std::uint8_t> {
	public:
		using CqlProtocolIntegerBase::get;
		using CqlProtocolIntegerBase::set;
		using CqlProtocolIntegerBase::encode;
		using CqlProtocolIntegerBase::decode;
		using CqlProtocolIntegerBase::CqlProtocolIntegerBase;
	};
}

