#pragma once
#include "CqlListBase.hpp"

namespace cql {
	/** List type uses std::vector */
	template <class T>
	class CqlList : private CqlListBase<std::vector<T>> {
	public:
		using Base = CqlListBase<std::vector<T>>;
		using Base::CqlUnderlyingType;
		using Base::CqlValueType;
		using Base::get;
		using Base::set;
		using Base::reset;
		using Base::encodeBody;
		using Base::decodeBody;
		using Base::CqlListBase;
		using Base::operator*;
		using Base::operator->;
	};
}

