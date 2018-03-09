#pragma once
#include "./StringBase.hpp"

namespace cql {
	/**
	 * A sequence of bytes of conforming to the UTF-8 specifications.
	 * For now there no client side validation (for performance).
	 * Notice: "varchar" is an alias of the "text" type.
	 */
	class Text : private StringBase {
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

