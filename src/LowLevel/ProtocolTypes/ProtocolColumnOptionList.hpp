#pragma once
#include "./ProtocolListBase.hpp"
#include "./ProtocolColumnOption.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n [option]
	 */
	class ProtocolColumnOptionList :
		private ProtocolListBase<std::uint16_t, ProtocolColumnOption> {
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

