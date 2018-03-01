#pragma once
#include <cstdint>
#include <vector>
#include <initializer_list>
#include <core/sstring.hh>
#include "../Exceptions/CqlDecodeException.hpp"
#include "../CqlColumnTrait.hpp"
#include "CqlInt.hpp"

namespace cql {
	/**
	 * A [int] n indicating the number of elements in the list, followed by n elements.
	 * Each element is [bytes] representing the serialized value.
	 */
	template <class T>
	class CqlList {
	public:
		using CqlUnderlyingType = std::vector<T>;

		/** Get the list value */
		const CqlUnderlyingType& get() const& { return value_; }

		/** Get the mutable list value */
		CqlUnderlyingType& get() & { return value_; }

		/** Set the list value */
		void set(const CqlUnderlyingType& value) { value_ = value_; }

		/** Set the list value */
		void set(CqlUnderlyingType&& value) { value_ = std::move(value); }

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
		void encodeBody(seastar::sstring& data) const {
			CqlInt listSize(value_.size());
			listSize.encodeBody(data);
			for (const auto& item : value_) {
				CqlColumnTrait<T>::encode(item, data);
			}
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, ColumnEncodeDecodeSizeType size) {
			if (size < CqlInt::CqlEncodeSize) {
				throw CqlDecodeException(CQL_CODEINFO,
					"length of list size not enough, element type is:", typeid(T).name());
			}
			CqlInt listSize;
			listSize.decodeBody(ptr, CqlInt::CqlEncodeSize);
			if (listSize < 0) {
				throw CqlDecodeException(CQL_CODEINFO,
					"list size can't be negative:", listSize);
			}
			const char* ptrStart = ptr + CqlInt::CqlEncodeSize;
			const char* end = ptr + size;
			value_.resize(0);
			for (std::size_t index = 0; index < listSize; ++index) {
				value_.emplace_back();
				CqlColumnTrait<T>::decode(value_.back(), ptrStart, end);
			}
		}

		/** Constructors */
		CqlList() : value_() { }
		explicit CqlList(const CqlUnderlyingType& value) : value_(value) { }
		explicit CqlList(CqlUnderlyingType&& value) : value_(std::move(value)) { }
		template <class U>
		explicit CqlList(std::initializer_list<U>&& items) : value_() {
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

