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

		/** Construct a string_view by the content it holds */
		std::string_view get() const& {
			if (ref_.first != nullptr) {
				return std::string_view(ref_.first, ref_.second);
			} else {
				return std::string_view(str_);
			}
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
			static_assert(Size > 0, "size of c string should not be 0");
		}

		/** Compare operations */
		bool operator==(const StringHolder& other) const {
			auto a = get();
			auto b = other.get();
			if (a.size() == b.size() && a.data() == b.data()) {
				return true; // fast path, gcc won't optimize it
			}
			return a == b;
		}
		bool operator!=(const StringHolder& other) const {
			return !(*this == other);
		}

		/** Hash functor */
		struct Hash {
			std::size_t operator()(const StringHolder& value) const {
				return std::hash<std::string_view>()(value.get());
			}
		};

	private:
		std::string str_;
		std::pair<const char*, std::size_t> ref_;
	};

	/** Write the content to stream */
	static std::ostream& operator<<(std::ostream& stream, const StringHolder& value) {
		stream << value.get();
		return stream;
	}
}

