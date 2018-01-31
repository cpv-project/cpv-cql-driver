#pragma once
#include <chrono>
#include <type_traits>

namespace cql {
	template <class T, class Rep, class Period,
		std::enable_if_t<std::is_same<
			typename T::CqlUnderlyingType,
			std::chrono::system_clock::time_point>::value, int> = 0>
	T operator+(const T& a, const std::chrono::duration<Rep, Period>& b) {
		return T(a.get() + b);
	}

	template <class T, class Rep, class Period,
		std::enable_if_t<std::is_same<
			typename T::CqlUnderlyingType,
			std::chrono::system_clock::time_point>::value, int> = 0>
	T operator+(const std::chrono::duration<Rep, Period>& a, const T& b) {
		return T(a + b.get()); // this variant is addition only
	}

	template <class T, class Rep, class Period,
		std::enable_if_t<std::is_same<
			typename T::CqlUnderlyingType,
			std::chrono::system_clock::time_point>::value, int> = 0>
	T operator-(const T& a, const std::chrono::duration<Rep, Period>& b) {
		return T(a.get() - b);
	}

	template <class T,
		std::enable_if_t<std::is_same<
			typename T::CqlUnderlyingType,
			std::chrono::system_clock::time_point>::value, int> = 0>
	std::chrono::system_clock::duration operator-(const T& a, const T& b) {
		return a.get() - b.get(); // this variant is subtraction only
	}

#define DefineCqlDateTimeCompareOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_same< \
			typename TA::CqlUnderlyingType, \
			std::chrono::system_clock::time_point>::value, int> = 0, \
		std::enable_if_t<std::is_same< \
			typename TB::CqlUnderlyingType, \
			std::chrono::system_clock::time_point>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::CqlUnderlyingType, \
			std::chrono::system_clock::time_point>::value, int> = 0> \
	bool operator op(const T& a, const typename T::CqlUnderlyingType& b) { \
		return a.get() op b; \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::CqlUnderlyingType, \
			std::chrono::system_clock::time_point>::value, int> = 0> \
	bool operator op(const typename T::CqlUnderlyingType& a, const T& b) { \
		return a op b.get(); \
	}

	DefineCqlDateTimeCompareOperation(==)
	DefineCqlDateTimeCompareOperation(!=)
	DefineCqlDateTimeCompareOperation(>)
	DefineCqlDateTimeCompareOperation(<)
	DefineCqlDateTimeCompareOperation(>=)
	DefineCqlDateTimeCompareOperation(<=)

#undef DefineCqlDateTimeCompareOperation

#define DefineCqlDateTimeCompoundAssignOperation(op) \
	template <class T, class Rep, class Period, \
		std::enable_if_t<std::is_same< \
			typename T::CqlUnderlyingType, \
			std::chrono::system_clock::time_point>::value, int> = 0> \
		T& operator op(T& a, const std::chrono::duration<Rep, Period>& b) { \
			a.get() op b; \
			return a; \
		}

	DefineCqlDateTimeCompoundAssignOperation(+=)
	DefineCqlDateTimeCompoundAssignOperation(-=)

#undef DefineCqlDateTimeCompoundAssignOperation
}

