#pragma once
#include "CqlProtocolMapBase.hpp"
#include "CqlProtocolString.hpp"
#include "CqlProtocolBytes.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n pair <k><v> where <k> is a [string] and <v> is a [bytes]
	 */
	class CqlProtocolBytesMap :
		protected CqlProtocolMapBase<std::uint16_t, CqlProtocolString, CqlProtocolBytes> {
	public:
		using CqlProtocolMapBase::get;
		using CqlProtocolMapBase::encode;
		using CqlProtocolMapBase::decode;
		using CqlProtocolMapBase::CqlProtocolMapBase;
	};
}

