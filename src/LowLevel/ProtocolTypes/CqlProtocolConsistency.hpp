#pragma once
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>
#include "CqlProtocolIntegerBase.hpp"

namespace cql {
	/**
	 * A consistency level specification
	 * This is a [short] representing a consistency level.
	 */
	class CqlProtocolConsistency : private CqlProtocolIntegerBase<std::uint16_t> {
	public:
		using CqlProtocolIntegerBase::encode;
		using CqlProtocolIntegerBase::decode;

		CqlConsistencyLevel get() const { return static_cast<CqlConsistencyLevel>(value_); }
		void set(CqlConsistencyLevel level) { value_ = static_cast<decltype(value_)>(level); }

		explicit CqlProtocolConsistency(CqlConsistencyLevel level) { set(level); }
		CqlProtocolConsistency() : CqlProtocolConsistency(CqlConsistencyLevel::Any) { }
	};
}

