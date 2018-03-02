#pragma once
#include <cstdint>
#include <initializer_list>
#include <core/sstring.hh>
#include "../Exceptions/CqlDecodeException.hpp"
#include "../CqlColumnTrait.hpp"
#include "CqlInt.hpp"

namespace cql {
	/**
	 * A [int] n indicating the number of key/value pairs in the map, followed by n entries.
	 * Each entry is composed of two [bytes] representing the key and value.
	 */
	template <class MapType>
	class CqlMapBase {
	public:
		using CqlUnderlyingType = MapType;
		using CqlKeyType = typename MapType::key_type;
		using CqlMappedType = typename MapType::mapped_type;
		using CqlValueType = typename MapType::value_type;

		/** Get the map value */
		const CqlUnderlyingType& get() const& { return value_; }

		/** Get the mutable map value */
		CqlUnderlyingType& get() & { return value_; }

		/** Set the map value */
		void set(const CqlUnderlyingType& value) { value_ = value_; }

		/** Set the map value */
		void set(CqlUnderlyingType&& value) { value_ = std::move(value); }

		/** Set the map value by initializer list */
		template <class UKey, class UValue>
		void set(std::initializer_list<std::pair<UKey, UValue>>&& items) {
			value_.clear();
			for (auto&& item : items) {
				value_.emplace(
					CqlKeyType(std::move(item.first)),
					CqlMappedType(std::move(item.second)));
			}
		}

		/** Reset to initial state */
		void reset() { value_.clear(); }

		/** Encode to binary data */
		void encodeBody(seastar::sstring& data) const {
			CqlInt mapSize(value_.size());
			mapSize.encodeBody(data);
			for (const auto& item : value_) {
				CqlColumnTrait<CqlKeyType>::encode(item.first, data);
				CqlColumnTrait<CqlMappedType>::encode(item.second, data);
			}
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, ColumnEncodeDecodeSizeType size) {
			if (size < CqlInt::CqlEncodeSize) {
				throw CqlDecodeException(CQL_CODEINFO,
					"length of map size not enough, element type is:",
					typeid(CqlValueType).name());
			}
			CqlInt mapSize;
			mapSize.decodeBody(ptr, CqlInt::CqlEncodeSize);
			if (mapSize < 0) {
				throw CqlDecodeException(CQL_CODEINFO,
					"map size can't be negative:", mapSize);
			}
			const char* ptrStart = ptr + CqlInt::CqlEncodeSize;
			const char* end = ptr + size;
			value_.clear();
			for (std::size_t index = 0; index < mapSize; ++index) {
				CqlKeyType key;
				CqlMappedType value;
				CqlColumnTrait<CqlKeyType>::decode(key, ptrStart, end);
				CqlColumnTrait<CqlMappedType>::decode(value, ptrStart, end);
				value_.emplace(std::move(key), std::move(value));
			}
		}

		/** Constructors */
		CqlMapBase() : value_() { }
		explicit CqlMapBase(const CqlUnderlyingType& value) : value_(value) { }
		explicit CqlMapBase(CqlUnderlyingType&& value) : value_(std::move(value)) { }
		template <class UKey, class UValue>
		explicit CqlMapBase(std::initializer_list<std::pair<UKey, UValue>>&& items) : value_() {
			set(std::move(items));
		}

		/** Dereference operation */
		const CqlUnderlyingType& operator*() const& { return value_; }
		CqlUnderlyingType& operator*() & { return value_; }

		/** Get pointer operation */
		const CqlUnderlyingType* operator->() const& { return &value_; }
		CqlUnderlyingType* operator->() & { return &value_; }

	private:
		CqlUnderlyingType value_;
	};
}

