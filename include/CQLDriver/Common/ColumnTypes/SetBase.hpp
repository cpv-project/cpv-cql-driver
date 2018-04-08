#pragma once
#include <cstdint>
#include <initializer_list>
#include "../Exceptions/DecodeException.hpp"
#include "../ColumnTrait.hpp"
#include "./Int.hpp"

namespace cql {
	/**
	 * A [int] n indicating the number of elements in the set, followed by n elements.
	 * Each element is [bytes] representing the serialized value.
	 */
	template <class SetType>
	class SetBase {
	public:
		using UnderlyingType = SetType;
		using ValueType = typename SetType::value_type;

		/** Get the set value */
		const UnderlyingType& get() const& { return value_; }

		/** Get the mutable set value */
		UnderlyingType& get() & { return value_; }

		/** Set the set value */
		void set(const UnderlyingType& value) { value_ = value_; }

		/** Set the set value */
		void set(UnderlyingType&& value) { value_ = std::move(value); }

		/** Set the set value by initializer list */
		template <class U>
		void set(std::initializer_list<U>&& items) {
			value_.clear();
			for (auto&& item : items) {
				value_.emplace(std::move(item));
			}
		}

		/** Reset to initial state */
		void reset() { value_.clear(); }

		/** Encode to binary data */
		void encodeBody(std::string& data) const {
			Int setSize(value_.size());
			setSize.encodeBody(data);
			for (const auto& item : value_) {
				ColumnTrait<ValueType>::encode(item, data);
			}
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, ColumnEncodeDecodeSizeType size) {
			if (size < static_cast<ColumnEncodeDecodeSizeType>(Int::EncodeSize)) {
				throw DecodeException(CQL_CODEINFO,
					"length of set size not enough, element type is:",
					typeid(ValueType).name());
			}
			Int setSize;
			setSize.decodeBody(ptr, Int::EncodeSize);
			if (setSize < 0) {
				throw DecodeException(CQL_CODEINFO,
					"set size can't be negative:", setSize);
			}
			const char* ptrStart = ptr + Int::EncodeSize;
			const char* end = ptr + size;
			value_.clear();
			for (Int::UnderlyingType index = 0; index < setSize; ++index) {
				ValueType item;
				ColumnTrait<ValueType>::decode(item, ptrStart, end);
				value_.emplace(std::move(item));
			}
		}

		/** Constructors */
		SetBase() : value_() { }
		explicit SetBase(const UnderlyingType& value) : value_(value) { }
		explicit SetBase(UnderlyingType&& value) : value_(std::move(value)) { }
		template <class U>
		explicit SetBase(std::initializer_list<U>&& items) : value_() {
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

