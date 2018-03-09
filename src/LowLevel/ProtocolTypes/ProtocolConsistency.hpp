#pragma once
#include <CQLDriver/Common/CommonDefinitions.hpp>
#include "./ProtocolIntegerBase.hpp"

namespace cql {
	/**
	 * A consistency level specification
	 * This is a [short] representing a consistency level.
	 */
	class ProtocolConsistency : private ProtocolIntegerBase<std::uint16_t> {
	public:
		using ProtocolIntegerBase::reset;
		using ProtocolIntegerBase::encode;
		using ProtocolIntegerBase::decode;

		ConsistencyLevel get() const { return static_cast<ConsistencyLevel>(value_); }
		void set(ConsistencyLevel level) { value_ = static_cast<decltype(value_)>(level); }

		explicit ProtocolConsistency(ConsistencyLevel level) { set(level); }
		ProtocolConsistency() : ProtocolConsistency(ConsistencyLevel::Any) { }
	};
}

