#pragma once
#include "./ProtocolIntegerBase.hpp"

namespace cql {
	/** A 4 bytes signed integer */
	class ProtocolInt : private ProtocolIntegerBase<std::int32_t> {
	public:
		using ProtocolIntegerBase::get;
		using ProtocolIntegerBase::set;
		using ProtocolIntegerBase::reset;
		using ProtocolIntegerBase::encode;
		using ProtocolIntegerBase::decode;
		using ProtocolIntegerBase::ProtocolIntegerBase;
	};
}

