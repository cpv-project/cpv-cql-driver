#pragma once
#include <unordered_set>
#include "./SetBase.hpp"

namespace cql {
	/** Set type uses std::unordered_set */
	template <class T>
	class Set :
		private SetBase<std::unordered_set<T, typename ColumnTrait<T>::Hash>> {
	public:
		using Base = SetBase<std::unordered_set<T, typename ColumnTrait<T>::Hash>>;
		using Base::UnderlyingType;
		using Base::ValueType;
		using Base::get;
		using Base::set;
		using Base::reset;
		using Base::encodeBody;
		using Base::decodeBody;
		using Base::SetBase;
		using Base::operator*;
		using Base::operator->;
	};
}

