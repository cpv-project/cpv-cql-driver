#pragma once
#include "CqlProtocolIntegerBase.hpp"

namespace cql {
	/** A 2 bytes unsigned integer */
	class CqlProtocolShort : private CqlProtocolIntegerBase<std::uint16_t> {
	public:
		using CqlProtocolIntegerBase::get;
		using CqlProtocolIntegerBase::set;
		using CqlProtocolIntegerBase::encode;
		using CqlProtocolIntegerBase::decode;
		using CqlProtocolIntegerBase::CqlProtocolIntegerBase;
	};
}

