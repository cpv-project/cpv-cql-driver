#pragma once
#include "CqlProtocolVariableIntegerBase.hpp"

namespace cql {
	/**
	 * A unsigned variable length integer
	 * See the comments in CqlProtocolVariableIntegerBase
	 */
	class CqlProtocolUnsignedVint : private CqlProtocolVariableIntegerBase {
	public:
		using CqlProtocolVariableIntegerBase::get;
		using CqlProtocolVariableIntegerBase::set;
		using CqlProtocolVariableIntegerBase::reset;
		using CqlProtocolVariableIntegerBase::encode;
		using CqlProtocolVariableIntegerBase::decode;
		using CqlProtocolVariableIntegerBase::CqlProtocolVariableIntegerBase;
	};
}

