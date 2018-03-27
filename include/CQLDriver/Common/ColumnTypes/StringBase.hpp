#pragma once
#include "./ColumnDefinitions.hpp"
#include "./StringOperations.hpp"

namespace cql {
	/** Base class of string types */
	class StringBase {
	public:
		using UnderlyingType = std::string;

		/** Get the value of string */
		const std::string& get() const& { return value_; }

		/** Get the mutable value of string */
		std::string& get() & { return value_; }

		/** Set the value of string */
		void set(const char* str, const std::size_t size) {
			value_.resize(0);
			value_.append(str, size);
		}

		/** Set the value of string */
		template <std::size_t Size>
		void set(const char(&str)[Size]) {
			static_assert(Size > 0, "size of c string should not be 0");
			value_.resize(0);
			value_.append(str, Size - 1);
		}

		/** Set the value of string */
		void set(const std::string& value) {
			value_ = value;
		}

		/** Set the value of string */
		void set(std::string&& value) {
			value_ = std::move(value);
		}

		/** Append the value of string */
		void append(const char* str, std::size_t size) {
			value_.append(str, size);
		}

		/** Append the value of string */
		template <std::size_t Size>
		void append(const char(&str)[Size]) {
			static_assert(Size > 0, "size of c string should not be 0");
			value_.append(str, Size - 1);
		}

		/** Reset to initial state */
		void reset() {
			value_.resize(0);
		}

		/** Get string data */
		const char* c_str() const& { return value_.c_str(); }
		const char* data() const& { return value_.data(); }
		char* data() & { return &value_[0]; }

		/** Get length of string */
		std::size_t size() const { return value_.size(); }

		/** Get whether string is empty */
		bool empty() const { return value_.empty(); }

		/** Encode to binary data */
		void encodeBody(std::string& data) const {
			data.append(value_.data(), value_.size());
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, ColumnEncodeDecodeSizeType size) {
			if (size >= 0) {
				set(ptr, static_cast<std::size_t>(size));
			}
		}

		/** Constructor */
		StringBase() : value_() { }
		template <class... Args>
		// cppcheck-suppress noExplicitConstructor
		StringBase(Args&&... args) : value_() {
			set(std::forward<Args>(args)...);
		}

		/** Allow assign from string */
		template <class... Args>
		void operator=(Args&&... args) {
			set(std::forward<Args>(args)...);
		}

		/** Allow cast to string implicitly */
		operator const std::string&() const& { return value_; }
		operator std::string&() & { return value_; }

		/** Dereference operation */
		const std::string& operator*() const& { return value_; }
		std::string& operator*() & { return value_; }

		/** Get pointer operation */
		const std::string* operator->() const& { return &value_; }
		std::string* operator->() & { return &value_; }

	protected:
		std::string value_;
	};

	/** Write the string content to stream */
	template <class T,
		std::enable_if_t<std::is_same<
			typename T::UnderlyingType, std::string>::value, int> = 0>
	std::ostream& operator<<(std::ostream& stream, const T& value) {
		stream << value.get();
		return stream;
	}
}

