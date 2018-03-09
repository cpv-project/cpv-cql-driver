#pragma once
#include <unordered_map>
#include "./MapBase.hpp"

namespace cql {
	/** Map type uses std::unordered_map */
	template <class TKey, class TValue>
	class Map :
		private MapBase<
			std::unordered_map<TKey, TValue, typename ColumnTrait<TKey>::Hash>> {
	public:
		using Base = MapBase<
			std::unordered_map<TKey, TValue, typename ColumnTrait<TKey>::Hash>>;
		using Base::UnderlyingType;
		using Base::KeyType;
		using Base::MappedType;
		using Base::ValueType;
		using Base::get;
		using Base::set;
		using Base::reset;
		using Base::encodeBody;
		using Base::decodeBody;
		using Base::MapBase;
		using Base::operator*;
		using Base::operator->;
	};
}

