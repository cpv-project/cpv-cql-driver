#pragma once
#include <utility>
#include <string>
#include "./ColumnDefinitions.hpp"

namespace cql {
	/**
	 * Wrapper class that able to represent a null value
	 * For convenient and performance, getting underlying value will not perform null check,
	 * to check it is null explicitly, use function isNull.
	 */
	template <class T>
	class Nullable {
	public:
		using UnderlyingType = T;

		/** Get underlying value without null check */
		const T& get() const& { return value_; }
		T& get() & { return value_; }

		/** Set underlying value */
		template <class... Args>
		void set(Args&&... args) {
			value_.set(std::forward<Args>(args)...);
			isNull_ = false;
		}

		/** Reset to null value */
		void reset() { value_.reset(); isNull_ = true; }

		/** Check whether it's a null value */
		bool isNull() const { return isNull_; }

		/** Encode to binary data */
		void encodeBody(std::string& data) {
			if (!isNull_) {
				value_.encodeBody(data);
			}
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, const ColumnEncodeDecodeSizeType& size) {
			if (size >= 0) {
				value_.decodeBody(ptr, size);
				isNull_ = false;
			} else {
				reset();
			}
		}

		/** Constructor */
		Nullable() : value_(), isNull_(true) { }
		template <class... Args>
		explicit Nullable(Args&&... args) :
			value_(std::forward<Args>(args)...), isNull_(false) { }

		/** Dereference without null check */
		const T& operator*() const& { return value_; }
		T& operator*() & { return value_; }

		/** Get pointer without null check */
		const T* operator->() const& { return &value_; }
		T* operator->() & { return &value_; }

	private:
		T value_;
		bool isNull_;
	};
}

