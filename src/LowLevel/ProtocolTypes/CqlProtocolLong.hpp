#pragma once
#include "CqlProtocolIntegerBase.hpp"

namespace cql {
	/** A 8 bytes signed integer */
	class CqlProtocolLong : protected CqlProtocolIntegerBase<std::int64_t> {
	public:
		using CqlProtocolIntegerBase::get;
		using CqlProtocolIntegerBase::set;
		using CqlProtocolIntegerBase::encode;
		using CqlProtocolIntegerBase::decode;
		using CqlProtocolIntegerBase::CqlProtocolIntegerBase;
	};
}

