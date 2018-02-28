#pragma once
#include <utility>
#include <core/sstring.hh>

namespace cql {
	/**
	 * Wrapper class that able to represent a null value
	 * For convenient and performance, getting underlying value will not perform null check,
	 * to check it is null explicitly, use function isNull.
	 */
	template <class T>
	class CqlNullable {
	public:
		using CqlUnderlyingType = T;
		static const bool CqlIsNullable = true;

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
		void encode(seastar::sstring& data) {
			if (!isNull_) {
				value_.encode(data);
			}
		}

		/** Decode from binary data */
		void decode(const char* ptr, std::int32_t size) {
			if (size >= 0) {
				value_.decode(ptr, size);
				isNull_ = false;
			} else {
				reset();
			}
		}

		/** Constructor */
		CqlNullable() : value_(), isNull_(true) { }
		template <class... Args>
		CqlNullable(Args&&... args) :
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

