#pragma once
#include "CqlProtocolIntegerBase.hpp"

namespace cql {
	/**
	 * A consistency level specification
	 * This is a [short] representing a consistency level.
	 */
	class CqlProtocolConsistency : protected CqlProtocolIntegerBase<std::uint16_t> {
	public:
		using CqlProtocolIntegerBase::get;
		using CqlProtocolIntegerBase::set;
		using CqlProtocolIntegerBase::encode;
		using CqlProtocolIntegerBase::decode;
		using CqlProtocolIntegerBase::CqlProtocolIntegerBase;
	};
}

