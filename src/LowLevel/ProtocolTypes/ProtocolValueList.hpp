#pragma once
#include "./ProtocolListBase.hpp"
#include "./ProtocolValue.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n [value]
	 */
	class ProtocolValueList :
		private ProtocolListBase<std::uint16_t, ProtocolValue> {
	public:
		using ProtocolListBase::SmallSizeBoundary;
		using ProtocolListBase::get;
		using ProtocolListBase::set;
		using ProtocolListBase::reset;
		using ProtocolListBase::encode;
		using ProtocolListBase::decode;
		using ProtocolListBase::ProtocolListBase;
	};
}

