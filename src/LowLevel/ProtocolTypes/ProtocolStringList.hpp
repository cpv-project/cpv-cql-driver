#pragma once
#include "./ProtocolListBase.hpp"
#include "./ProtocolString.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n [string]
	 */
	class ProtocolStringList :
		private ProtocolListBase<std::uint16_t, ProtocolString> {
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

