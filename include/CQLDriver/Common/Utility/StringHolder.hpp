#pragma once
#include <cstring>
#include <string>
#include <utility>
#include <iostream>
#include <string_view>

namespace cql {
	/**
	 * Class used to hold a string reference or a string.
	 * It can be used to avoid unnecessary memory copy.
	 * If you use it to store string reference,
	 * ensure the lifetime of StringHolder is shorter than the original content.
	 */
	class StringHolder {
	public:
		/** Check whether it has the ownership of the string content */
		bool hasOwnership() const {
			return ref_.first == nullptr;
		}

		/** Get the address and size of string */
		std::pair<const char*, std::size_t> get() const& {
			if (ref_.first != nullptr) {
				return ref_;
			} else {
				return { str_.data(), str_.size() };
			}
		}

		/** Construct a string_view by the content it holds */
		std::string_view getStringView() const {
			auto addressAndSize = get();
			return std::string_view(addressAndSize.first, addressAndSize.second);
		}

		/** Constructors */
		StringHolder() :
			str_(),
			ref_(nullptr, 0) { }
		explicit StringHolder(std::string&& str) :
			str_(std::move(str)),
			ref_(nullptr, 0) { }
		explicit StringHolder(const char* ptr, std::size_t size) :
			str_(),
			ref_(ptr, size) { }
		template <std::size_t Size>
		explicit StringHolder(const char(&ptr)[Size]) :
			StringHolder(ptr, Size-1) {
			static_assert(Size > 0, "size of c string is 0");
		}

		/** Compare operations */
		bool operator==(const StringHolder& other) const {
			auto a = get();
			auto b = other.get();
			if (a.second != b.second) {
				return false;
			} else if (a.first == b.first) {
				return true; // fast path
			}
			return std::memcmp(a.first, b.first, a.second) == 0;
		}
		bool operator!=(const StringHolder& other) const {
			return !(*this == other);
		}

		/** Hash functor */
		struct Hash {
			std::size_t operator()(const StringHolder& value) const {
				return std::hash<std::string_view>()(value.getStringView());
			}
		};

	private:
		std::string str_;
		std::pair<const char*, std::size_t> ref_;
	};

	/** Write the content to stream */
	static std::ostream& operator<<(std::ostream& stream, const StringHolder& value) {
		stream << value.getStringView();
		return stream;
	}
}

