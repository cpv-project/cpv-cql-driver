#pragma once
#include "CqlProtocolListBase.hpp"
#include "CqlProtocolValue.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n [value]
	 */
	class CqlProtocolValueList :
		private CqlProtocolListBase<std::uint16_t, CqlProtocolValue> {
	public:
		using CqlProtocolListBase::SmallSizeBoundary;
		using CqlProtocolListBase::get;
		using CqlProtocolListBase::set;
		using CqlProtocolListBase::reset;
		using CqlProtocolListBase::encode;
		using CqlProtocolListBase::decode;
		using CqlProtocolListBase::CqlProtocolListBase;
	};
}

