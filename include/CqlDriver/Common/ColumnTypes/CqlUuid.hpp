#pragma once
#include "CqlUuidBase.hpp"

namespace cql {
	/** A 16 byte sequence representing any valid UUID as defined by RFC 4122 */
	class CqlUuid : private CqlUuidBase {
	public:
		using CqlUuidBase::CqlUnderlyingType;
		using CqlUuidBase::CqlEncodeSize;
		using CqlUuidBase::get;
		using CqlUuidBase::set;
		using CqlUuidBase::reset;
		using CqlUuidBase::empty;
		using CqlUuidBase::str;
		using CqlUuidBase::encodeBody;
		using CqlUuidBase::decodeBody;
		using CqlUuidBase::CqlUuidBase;
		using CqlUuidBase::operator=;
		using CqlUuidBase::operator CqlUnderlyingType;

		/** Create a version 4 (random) uuid */
		static CqlUuid create() { return CqlUuid(makeRandomUuid()); }
	};
}

