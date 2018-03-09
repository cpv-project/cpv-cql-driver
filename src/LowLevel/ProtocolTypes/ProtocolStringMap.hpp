#pragma once
#include "./ProtocolMapBase.hpp"
#include "./ProtocolString.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n pair <k><v> where <k> and <v> are [string]
	 */
	class ProtocolStringMap :
		private ProtocolMapBase<std::uint16_t, ProtocolString, ProtocolString> {
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

