#pragma once
#include <cstdint>
#include <utility>
#include <iostream>
#include <string_view>
#include "./ColumnDefinitions.hpp"

namespace cql {
	/** Memory reference, it doesn't have the owership of the target */
	class MemRef {
	public:
		using UnderlyingType = std::pair<const char*, std::size_t>;

		/** Get the value of the reference */
		const UnderlyingType& get() const& { return value_; }

		/** Get the mutable value of the reference */
		UnderlyingType& get() & { return value_; }

		/** Set the value of the reference */
		void set(const UnderlyingType& value) {
			value_ = value;
		}

		/** Set the value of the reference */
		void set(const char* ptr, const std::size_t size) {
			value_ = { ptr, size };
		}

		/** Set the value of the reference */
		template <std::size_t Size>
		void set(const char(&ptr)[Size]) {
			static_assert(Size > 0, "size of c string should not be 0");
			value_ = { ptr, Size - 1 };
		}

		/** Get the pointer of the reference */
		const char* data() const { return value_.first; }

		/** Get the size of the reference */
		std::size_t size() const { return value_.second; }

		/** Get whether this reference is empty */
		bool empty() const { return value_.second == 0; }

		/** Reset to initial state */
		void reset() {
			value_ = { nullptr, 0 };
		}

		/** Encode to binary data */
		void encodeBody(std::string& data) const {
			data.append(value_.first, value_.second);
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, const ColumnEncodeDecodeSizeType& size) {
			if (size >= 0) {
				value_ = { ptr, static_cast<std::size_t>(size) };
			}
		}

		/** Constructor */
		MemRef() : value_(nullptr, 0) { }
		template <class... Args>
		// cppcheck-suppress noExplicitConstructor
		MemRef(Args&&... args) : value_() {
			set(std::forward<Args>(args)...);
		}

	private:
		UnderlyingType value_;
	};

	/** Write the referenced content to stream */
	static std::ostream& operator<<(std::ostream& stream, const MemRef& value) {
		stream << std::string_view(value.data(), value.size());
		return stream;
	}
}

