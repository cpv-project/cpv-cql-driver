#pragma once
#include "./StringBase.hpp"

namespace cql {
	/**
	 * A sequence of bytes in the ASCII range [0, 127].
	 * For now there no client side validation (for performance).
	 */
	class Ascii : private StringBase {
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

