#pragma once
#include "CqlStringBase.hpp"

namespace cql {
	/**
	 * Any sequence of bytes
	 */
	class CqlBlob : private CqlStringBase {
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

