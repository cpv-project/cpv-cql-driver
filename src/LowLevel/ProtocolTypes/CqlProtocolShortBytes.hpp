#pragma once
#include <cstdint>
#include <core/sstring.hh>
#include "CqlProtocolString.hpp"

namespace cql {
	/**
	 * A [short] n, followed by n bytes if n >= 0,
	 * different to CqlProtocolBytes, CqlProtocolShortBytes can't represent `null`
	 */
	class CqlProtocolShortBytes : private CqlProtocolString {
	public:
		using CqlProtocolString::LengthType;
		using CqlProtocolString::get;
		using CqlProtocolString::encode;
		using CqlProtocolString::decode;
		using CqlProtocolString::CqlProtocolString;
	};
}

