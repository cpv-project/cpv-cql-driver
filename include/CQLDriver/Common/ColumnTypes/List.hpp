#pragma once
#include <vector>
#include "./ListBase.hpp"

namespace cql {
	/** List type uses std::vector */
	template <class T>
	class List : private ListBase<std::vector<T>> {
	public:
		using Base = ListBase<std::vector<T>>;
		using Base::UnderlyingType;
		using Base::ValueType;
		using Base::get;
		using Base::set;
		using Base::reset;
		using Base::encodeBody;
		using Base::decodeBody;
		using Base::ListBase;
		using Base::operator*;
		using Base::operator->;
	};
}

