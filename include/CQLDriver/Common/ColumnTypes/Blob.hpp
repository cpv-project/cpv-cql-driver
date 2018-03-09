#pragma once
#include "./StringBase.hpp"

namespace cql {
	/**
	 * Any sequence of bytes
	 */
	class Blob : private StringBase {
	public:
		using StringBase::UnderlyingType;
		using StringBase::get;
		using StringBase::set;
		using StringBase::append;
		using StringBase::reset;
		using StringBase::c_str;
		using StringBase::data;
		using StringBase::size;
		using StringBase::empty;
		using StringBase::encodeBody;
		using StringBase::decodeBody;
		using StringBase::StringBase;
		using StringBase::operator=;
		using StringBase::operator const UnderlyingType&;
		using StringBase::operator UnderlyingType&;
		using StringBase::operator*;
		using StringBase::operator->;
	};
}

