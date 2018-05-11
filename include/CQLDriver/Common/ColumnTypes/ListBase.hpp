#pragma once
#include <cstdint>
#include <initializer_list>
#include "../Exceptions/DecodeException.hpp"
#include "../ColumnTrait.hpp"
#include "./Int.hpp"

namespace cql {
	/**
	 * A [int] n indicating the number of elements in the list, followed by n elements.
	 * Each element is [bytes] representing the serialized value.
	 */
	template <class ListType>
	class ListBase {
	public:
		using UnderlyingType = ListType;
		using ValueType = typename ListType::value_type;

		/** Get the list value */
		const UnderlyingType& get() const& { return value_; }

		/** Get the mutable list value */
		UnderlyingType& get() & { return value_; }

		/** Set the list value */
		void set(const UnderlyingType& value) { value_ = value_; }

		/** Set the list value */
		void set(UnderlyingType&& value) { value_ = std::move(value); }

		/** Set the list value by initializer list */
		template <class U>
		void set(std::initializer_list<U>&& items) {
			value_.resize(items.size());
			for (std::size_t i = 0; i < items.size(); ++i) {
				value_[i].set(std::move(items.begin()[i]));
			}
		}

		/** Reset to initial state */
		void reset() { value_.resize(0); }

		/** Encode to binary data */
		void encodeBody(std::string& data) const {
			Int listSize(value_.size());
			listSize.encodeBody(data);
			for (const auto& item : value_) {
				ColumnTrait<ValueType>::encode(item, data);
			}
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, const ColumnEncodeDecodeSizeType& size) {
			if (size < static_cast<ColumnEncodeDecodeSizeType>(Int::EncodeSize)) {
				throw DecodeException(CQL_CODEINFO,
					"length of list size not enough, element type is:",
					typeid(ValueType).name());
			}
			Int listSize;
			listSize.decodeBody(ptr, Int::EncodeSize);
			if (listSize < 0) {
				throw DecodeException(CQL_CODEINFO,
					"list size can't be negative:", listSize);
			}
			const char* ptrStart = ptr + Int::EncodeSize;
			const char* end = ptr + size;
			value_.resize(0);
			for (Int::UnderlyingType index = 0; index < listSize; ++index) {
				value_.emplace_back();
				ColumnTrait<ValueType>::decode(value_.back(), ptrStart, end);
			}
		}

		/** Constructors */
		ListBase() : value_() { }
		explicit ListBase(const UnderlyingType& value) : value_(value) { }
		explicit ListBase(UnderlyingType&& value) : value_(std::move(value)) { }
		template <class U>
		explicit ListBase(std::initializer_list<U>&& items) : value_() {
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

