#pragma once
#include <type_traits>

namespace cql {
	/** Bitwise or operation, for enum types under cql namespace and called by ADL */
	template <class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
	T operator|(T a, T b) {
		using t = std::underlying_type_t<T>;
		return static_cast<T>(static_cast<t>(a) | static_cast<t>(b));
	}

	/** Bitwise and operation */
	template <class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
	T operator&(T a, T b) {
		using t = std::underlying_type_t<T>;
		return static_cast<T>(static_cast<t>(a) & static_cast<t>(b));
	}

	/** Bitwise not operation */
	template <class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
	T operator~(T a) {
		using t = std::underlying_type_t<T>;
		return static_cast<T>(~static_cast<t>(a));
	}

	/**
	 * Test is enum value not equal to 0
	 * As operation bool can't be static so provide a standalone function.
	 */
	template <class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
	bool enumTrue(T a) {
		using t = std::underlying_type_t<T>;
		return static_cast<t>(a) != 0;
	}

	/** Test is enum value equal to 0 */
	template <class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
	bool enumFalse(T a) {
		using t = std::underlying_type_t<T>;
		return static_cast<t>(a) == 0;
	}

	/** Get the integer value of enum */
	template <class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
	std::underlying_type_t<T> enumValue(T a) {
		using t = std::underlying_type_t<T>;
		return static_cast<t>(a);
	}
}

