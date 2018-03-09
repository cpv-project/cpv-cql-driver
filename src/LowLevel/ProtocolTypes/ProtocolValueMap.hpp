#pragma once
#include "./ProtocolMapBase.hpp"
#include "./ProtocolString.hpp"
#include "./ProtocolValue.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n pair <k><v> where <k> is [string] and <v> is [value]
	 */
	class ProtocolValueMap :
		private ProtocolMapBase<std::uint16_t, ProtocolString, ProtocolValue> {
	public:
		using ProtocolMapBase::MapType;
		using ProtocolMapBase::get;
		using ProtocolMapBase::set;
		using ProtocolMapBase::reset;
		using ProtocolMapBase::encode;
		using ProtocolMapBase::decode;
		using ProtocolMapBase::ProtocolMapBase;
	};
}

