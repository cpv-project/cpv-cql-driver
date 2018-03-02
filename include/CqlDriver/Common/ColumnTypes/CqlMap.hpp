#pragma once
#include <unordered_map>
#include "CqlMapBase.hpp"

namespace cql {
	/** Map type uses std::unordered_map */
	template <class TKey, class TValue>
	class CqlMap :
		private CqlMapBase<std::unordered_map<TKey, TValue, typename CqlColumnTrait<TKey>::Hash>> {
	public:
		using Base = CqlMapBase<std::unordered_map<TKey, TValue, typename CqlColumnTrait<TKey>::Hash>>;
		using Base::CqlUnderlyingType;
		using Base::CqlValueType;
		using Base::get;
		using Base::set;
		using Base::reset;
		using Base::encodeBody;
		using Base::decodeBody;
		using Base::CqlMapBase;
		using Base::operator*;
		using Base::operator->;
	};
}

