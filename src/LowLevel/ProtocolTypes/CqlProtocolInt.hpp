#pragma once
#include "CqlProtocolIntegerBase.hpp"

namespace cql {
	/** A 4 bytes signed integer */
	class CqlProtocolInt : private CqlProtocolIntegerBase<std::int32_t> {
	public:
		using CqlProtocolIntegerBase::get;
		using CqlProtocolIntegerBase::set;
		using CqlProtocolIntegerBase::reset;
		using CqlProtocolIntegerBase::encode;
		using CqlProtocolIntegerBase::decode;
		using CqlProtocolIntegerBase::CqlProtocolIntegerBase;
	};
}

