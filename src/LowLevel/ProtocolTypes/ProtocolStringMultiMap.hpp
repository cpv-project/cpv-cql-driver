#pragma once
#include "./ProtocolMapBase.hpp"
#include "./ProtocolString.hpp"
#include "./ProtocolStringList.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n pair <k><v> where <k> is a [string] and <v> is a [string list]
	 */
	class ProtocolStringMultiMap :
		private ProtocolMapBase<std::uint16_t, ProtocolString, ProtocolStringList> {
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

