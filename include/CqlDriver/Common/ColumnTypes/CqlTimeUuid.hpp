#pragma once
#include "CqlUuidBase.hpp"

namespace cql {
	/** A 16 byte sequence representing a version 1 UUID as defined by RFC 4122 */
	class CqlTimeUuid : private CqlUuidBase {
	public:
		using CqlUuidBase::CqlUnderlyingType;
		using CqlUuidBase::get;
		using CqlUuidBase::set;
		using CqlUuidBase::reset;
		using CqlUuidBase::empty;
		using CqlUuidBase::str;
		using CqlUuidBase::encode;
		using CqlUuidBase::decode;
		using CqlUuidBase::CqlUuidBase;
		using CqlUuidBase::operator=;
		using CqlUuidBase::operator CqlUnderlyingType;

		/** Create a version 1 (date-time and MAC address) uuid */
		static CqlTimeUuid create() { return CqlTimeUuid(makeTimeUuid()); }
	};
}

