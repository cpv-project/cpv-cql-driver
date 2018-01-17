#pragma once
#include "CqlProtocolListBase.hpp"
#include "CqlProtocolBatchQuery.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n [batch_query]
	 */
	class CqlProtocolBatchQueryList :
		private CqlProtocolListBase<std::uint16_t, CqlProtocolBatchQuery> {
	public:
		using CqlProtocolListBase::SmallSizeBoundary;
		using CqlProtocolListBase::get;
		using CqlProtocolListBase::reset;
		using CqlProtocolListBase::encode;
		using CqlProtocolListBase::decode;
		using CqlProtocolListBase::CqlProtocolListBase;
	};
}

