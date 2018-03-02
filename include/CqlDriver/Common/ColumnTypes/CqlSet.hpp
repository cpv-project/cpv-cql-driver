#pragma once
#include <unordered_set>
#include "CqlSetBase.hpp"

namespace cql {
	/** Set type uses std::unordered_map */
	template <class T>
	class CqlSet :
		private CqlSetBase<std::unordered_set<T, typename CqlColumnTrait<T>::Hash>> {
	public:
		using Base = CqlSetBase<std::unordered_set<T, typename CqlColumnTrait<T>::Hash>>;
		using Base::CqlUnderlyingType;
		using Base::CqlValueType;
		using Base::get;
		using Base::set;
		using Base::reset;
		using Base::encodeBody;
		using Base::decodeBody;
		using Base::CqlSetBase;
		using Base::operator*;
		using Base::operator->;
	};
}

