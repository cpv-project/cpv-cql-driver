#pragma once
#include "CqlStringBase.hpp"

namespace cql {
	/**
	 * A sequence of bytes of conforming to the UTF-8 specifications.
	 * For now there no client side validation (for performance).
	 * Notice: "varchar" is an alias of the "text" type.
	 */
	class CqlText : private CqlStringBase {
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
		using CqlStringBase::encodeBody;
		using CqlStringBase::decodeBody;
		using CqlStringBase::CqlStringBase;
		using CqlStringBase::operator=;
		using CqlStringBase::operator const CqlUnderlyingType&;
		using CqlStringBase::operator CqlUnderlyingType&;
		using CqlStringBase::operator*;
		using CqlStringBase::operator->;
	};
}

