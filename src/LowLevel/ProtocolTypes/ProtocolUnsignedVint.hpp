#pragma once
#include "./ProtocolVariableIntegerBase.hpp"

namespace cql {
	/**
	 * A unsigned variable length integer
	 * See the comments in ProtocolVariableIntegerBase
	 */
	class ProtocolUnsignedVint : private ProtocolVariableIntegerBase {
	public:
		using ProtocolVariableIntegerBase::get;
		using ProtocolVariableIntegerBase::set;
		using ProtocolVariableIntegerBase::reset;
		using ProtocolVariableIntegerBase::encode;
		using ProtocolVariableIntegerBase::decode;
		using ProtocolVariableIntegerBase::ProtocolVariableIntegerBase;
	};
}

