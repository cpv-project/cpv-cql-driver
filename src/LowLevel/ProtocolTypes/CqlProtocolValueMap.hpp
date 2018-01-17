#pragma once
#include "CqlProtocolMapBase.hpp"
#include "CqlProtocolString.hpp"
#include "CqlProtocolValue.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n pair <k><v> where <k> is [string] and <v> is [value]
	 */
	class CqlProtocolValueMap :
		private CqlProtocolMapBase<std::uint16_t, CqlProtocolString, CqlProtocolValue> {
	public:
		using CqlProtocolMapBase::MapType;
		using CqlProtocolMapBase::get;
		using CqlProtocolMapBase::set;
		using CqlProtocolMapBase::reset;
		using CqlProtocolMapBase::encode;
		using CqlProtocolMapBase::decode;
		using CqlProtocolMapBase::CqlProtocolMapBase;
	};
}

