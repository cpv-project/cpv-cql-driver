#pragma once
#include "CqlProtocolMapBase.hpp"
#include "CqlProtocolString.hpp"
#include "CqlProtocolStringList.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n pair <k><v> where <k> is a [string] and <v> is a [string list]
	 */
	class CqlProtocolStringMultiMap :
		private CqlProtocolMapBase<std::uint16_t, CqlProtocolString, CqlProtocolStringList> {
	public:
		using CqlProtocolMapBase::get;
		using CqlProtocolMapBase::reset;
		using CqlProtocolMapBase::encode;
		using CqlProtocolMapBase::decode;
		using CqlProtocolMapBase::CqlProtocolMapBase;
	};
}

