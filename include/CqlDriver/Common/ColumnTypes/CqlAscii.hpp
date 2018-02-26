#pragma once
#include "CqlStringBase.hpp"

namespace cql {
	/**
	 * A sequence of bytes in the ASCII range [0, 127].
	 * For now there no client side validation (for performance).
	 */
	class CqlAscii : private CqlStringBase {
	public:
		using CqlStringBase::CqlUnderlyingType;
		using CqlStringBase::get;
		using CqlStringBase::set;
		using CqlStringBase::append;
		using CqlStringBase::reset;
		using CqlStringBase::c_str;
		using CqlStringBase::data;
		using CqlStringBase::size;
		using CqlStringBase::empty;
		using CqlStringBase::encode;
		using CqlStringBase::decode;
		using CqlStringBase::CqlStringBase;
		using CqlStringBase::operator=;
		using CqlStringBase::operator const CqlUnderlyingType&;
		using CqlStringBase::operator CqlUnderlyingType&;
	};
}

