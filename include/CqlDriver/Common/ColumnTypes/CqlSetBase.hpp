#pragma once
#include <cstdint>
#include <initializer_list>
#include <core/sstring.hh>
#include "../Exceptions/CqlDecodeException.hpp"
#include "../CqlColumnTrait.hpp"
#include "CqlInt.hpp"

namespace cql {
	/**
	 * A [int] n indicating the number of elements in the set, followed by n elements.
	 * Each element is [bytes] representing the serialized value.
	 */
	template <class SetType>
	class CqlSetBase {
	public:
		using CqlUnderlyingType = SetType;
		using CqlValueType = typename SetType::value_type;

		/** Get the set value */
		const CqlUnderlyingType& get() const& { return value_; }

		/** Get the mutable set value */
		CqlUnderlyingType& get() & { return value_; }

		/** Set the set value */
		void set(const CqlUnderlyingType& value) { value_ = value_; }

		/** Set the set value */
		void set(CqlUnderlyingType&& value) { value_ = std::move(value); }

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
		void encodeBody(seastar::sstring& data) const {
			CqlInt setSize(value_.size());
			setSize.encodeBody(data);
			for (const auto& item : value_) {
				CqlColumnTrait<CqlValueType>::encode(item, data);
			}
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, ColumnEncodeDecodeSizeType size) {
			if (size < CqlInt::CqlEncodeSize) {
				throw CqlDecodeException(CQL_CODEINFO,
					"length of set size not enough, element type is:",
					typeid(CqlValueType).name());
			}
			CqlInt setSize;
			setSize.decodeBody(ptr, CqlInt::CqlEncodeSize);
			if (setSize < 0) {
				throw CqlDecodeException(CQL_CODEINFO,
					"set size can't be negative:", setSize);
			}
			const char* ptrStart = ptr + CqlInt::CqlEncodeSize;
			const char* end = ptr + size;
			value_.clear();
			for (std::size_t index = 0; index < setSize; ++index) {
				CqlValueType item;
				CqlColumnTrait<CqlValueType>::decode(item, ptrStart, end);
				value_.emplace(item);
			}
		}

		/** Constructors */
		CqlSetBase() : value_() { }
		explicit CqlSetBase(const CqlUnderlyingType& value) : value_(value) { }
		explicit CqlSetBase(CqlUnderlyingType&& value) : value_(std::move(value)) { }
		template <class U>
		explicit CqlSetBase(std::initializer_list<U>&& items) : value_() {
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

