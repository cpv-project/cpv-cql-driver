#pragma once
#include <cstdint>
#include <initializer_list>
#include "../Exceptions/DecodeException.hpp"
#include "../ColumnTrait.hpp"
#include "./Int.hpp"

namespace cql {
	/**
	 * A [int] n indicating the number of key/value pairs in the map, followed by n entries.
	 * Each entry is composed of two [bytes] representing the key and value.
	 */
	template <class MapType>
	class MapBase {
	public:
		using UnderlyingType = MapType;
		using KeyType = typename MapType::key_type;
		using MappedType = typename MapType::mapped_type;
		using ValueType = typename MapType::value_type;

		/** Get the map value */
		const UnderlyingType& get() const& { return value_; }

		/** Get the mutable map value */
		UnderlyingType& get() & { return value_; }

		/** Set the map value */
		void set(const UnderlyingType& value) { value_ = value_; }

		/** Set the map value */
		void set(UnderlyingType&& value) { value_ = std::move(value); }

		/** Set the map value by initializer list */
		template <class UKey, class UValue>
		void set(std::initializer_list<std::pair<UKey, UValue>>&& items) {
			value_.clear();
			for (auto&& item : items) {
				value_.emplace(
					KeyType(std::move(item.first)),
					MappedType(std::move(item.second)));
			}
		}

		/** Reset to initial state */
		void reset() { value_.clear(); }

		/** Encode to binary data */
		void encodeBody(std::string& data) const {
			Int mapSize(value_.size());
			mapSize.encodeBody(data);
			for (const auto& item : value_) {
				ColumnTrait<KeyType>::encode(item.first, data);
				ColumnTrait<MappedType>::encode(item.second, data);
			}
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, ColumnEncodeDecodeSizeType size) {
			if (size < static_cast<ColumnEncodeDecodeSizeType>(Int::EncodeSize)) {
				throw DecodeException(CQL_CODEINFO,
					"length of map size not enough, element type is:",
					typeid(ValueType).name());
			}
			Int mapSize;
			mapSize.decodeBody(ptr, Int::EncodeSize);
			if (mapSize < 0) {
				throw DecodeException(CQL_CODEINFO,
					"map size can't be negative:", mapSize);
			}
			const char* ptrStart = ptr + Int::EncodeSize;
			const char* end = ptr + size;
			value_.clear();
			for (Int::UnderlyingType index = 0; index < mapSize; ++index) {
				KeyType key;
				MappedType value;
				ColumnTrait<KeyType>::decode(key, ptrStart, end);
				ColumnTrait<MappedType>::decode(value, ptrStart, end);
				value_.emplace(std::move(key), std::move(value));
			}
		}

		/** Constructors */
		MapBase() : value_() { }
		explicit MapBase(const UnderlyingType& value) : value_(value) { }
		explicit MapBase(UnderlyingType&& value) : value_(std::move(value)) { }
		template <class UKey, class UValue>
		explicit MapBase(std::initializer_list<std::pair<UKey, UValue>>&& items) : value_() {
			set(std::move(items));
		}

		/** Dereference operation */
		const UnderlyingType& operator*() const& { return value_; }
		UnderlyingType& operator*() & { return value_; }

		/** Get pointer operation */
		const UnderlyingType* operator->() const& { return &value_; }
		UnderlyingType* operator->() & { return &value_; }

	private:
		UnderlyingType value_;
	};
}

