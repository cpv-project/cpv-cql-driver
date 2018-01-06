#pragma once
#include "CqlProtocolIntegerBase.hpp"

namespace cql {
	/** A 4 bytes signed integer */
	class CqlProtocolInt : protected CqlProtocolIntegerBase<std::int32_t> {
	public:
		using CqlProtocolIntegerBase::get;
		using CqlProtocolIntegerBase::set;
		using CqlProtocolIntegerBase::encode;
		using CqlProtocolIntegerBase::decode;
		using CqlProtocolIntegerBase::CqlProtocolIntegerBase;
	};
}

