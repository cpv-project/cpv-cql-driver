#pragma once
#include "CqlProtocolListBase.hpp"
#include "CqlProtocolColumnOption.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n [option]
	 */
	class CqlProtocolColumnOptionList :
		protected CqlProtocolListBase<std::uint16_t, CqlProtocolColumnOption> {
	public:
		using CqlProtocolListBase::SmallSizeBoundary;
		using CqlProtocolListBase::get;
		using CqlProtocolListBase::encode;
		using CqlProtocolListBase::decode;
		using CqlProtocolListBase::CqlProtocolListBase;
	};
}

