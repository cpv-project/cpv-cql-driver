#pragma once
#include "CqlProtocolListBase.hpp"
#include "CqlProtocolString.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n [string]
	 */
	class CqlProtocolStringList :
		private CqlProtocolListBase<std::uint16_t, CqlProtocolString> {
	public:
		using CqlProtocolListBase::SmallSizeBoundary;
		using CqlProtocolListBase::get;
		using CqlProtocolListBase::reset;
		using CqlProtocolListBase::encode;
		using CqlProtocolListBase::decode;
		using CqlProtocolListBase::CqlProtocolListBase;
	};
}

