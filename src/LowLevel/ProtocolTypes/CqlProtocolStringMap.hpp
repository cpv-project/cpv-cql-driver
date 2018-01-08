#pragma once
#include "CqlProtocolMapBase.hpp"
#include "CqlProtocolString.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n pair <k><v> where <k> and <v> are [string]
	 */
	class CqlProtocolStringMap :
		private CqlProtocolMapBase<std::uint16_t, CqlProtocolString, CqlProtocolString> {
	public:
		using CqlProtocolMapBase::get;
		using CqlProtocolMapBase::encode;
		using CqlProtocolMapBase::decode;
		using CqlProtocolMapBase::CqlProtocolMapBase;
	};
}

