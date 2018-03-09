#pragma once
#include "./UUIDBase.hpp"

namespace cql {
	/** A 16 byte sequence representing any valid UUID as defined by RFC 4122 */
	class UUID : private UUIDBase {
	public:
		using UUIDBase::UnderlyingType;
		using UUIDBase::EncodeSize;
		using UUIDBase::get;
		using UUIDBase::set;
		using UUIDBase::reset;
		using UUIDBase::empty;
		using UUIDBase::str;
		using UUIDBase::encodeBody;
		using UUIDBase::decodeBody;
		using UUIDBase::UUIDBase;
		using UUIDBase::operator=;
		using UUIDBase::operator UnderlyingType;

		/** Create a version 4 (random) uuid */
		static UUID create() { return UUID(makeRandomUUID()); }
	};
}

