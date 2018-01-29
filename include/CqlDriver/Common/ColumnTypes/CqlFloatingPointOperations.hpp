#pragma once
#include <utility>
#include <type_traits>

namespace cql {
#define DefineCqlFloatingPointBinaryOperation(op) \
	template <class TA, class TB, \
		class TResult = std::conditional_t<sizeof(TA) >= sizeof(TB), TA, TB>, \
		std::enable_if_t<std::is_floating_point<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::CqlUnderlyingType>::value, int> = 0> \
	TResult operator op(const TA& a, const TB& b) { \
		return TResult(a.get() op b.get()); \
	} \
	template <class TA, class TB, \
		class TResult = TA, \
		std::enable_if_t<std::is_floating_point<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::CqlUnderlyingType>::value, int> = 0> \
	TResult operator op(const TA& a, const TB& b) { \
		return TResult(a.get() op b.get()); \
	} \
	template <class TA, class TB, \
		class TResult = TB, \
		std::enable_if_t<std::is_integral<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::CqlUnderlyingType>::value, int> = 0> \
	TResult operator op(const TA& a, const TB& b) { \
		return TResult(a.get() op b.get()); \
	} \
	template <class T, \
		std::enable_if_t<std::is_floating_point<typename T::CqlUnderlyingType>::value, int> = 0> \
	T operator op(const T& a, const typename T::CqlUnderlyingType& b) { \
		return T(a.get() op b); \
	} \
	template <class T, \
		std::enable_if_t<std::is_floating_point<typename T::CqlUnderlyingType>::value, int> = 0> \
	T operator op(const typename T::CqlUnderlyingType& a, const T& b) { \
		return T(a op b.get()); \
	}

	DefineCqlFloatingPointBinaryOperation(+)
	DefineCqlFloatingPointBinaryOperation(-)
	DefineCqlFloatingPointBinaryOperation(*)
	DefineCqlFloatingPointBinaryOperation(/)

#undef DefineCqlFloatingPointBinaryOperation

#define DefineCqlFloatingPointUnaryOperation(op) \
	template <class T, \
		std::enable_if_t<std::is_floating_point<typename T::CqlUnderlyingType>::value, int> = 0> \
	T operator op(const T& value) { \
		return T(op value.get()); \
	}

	DefineCqlFloatingPointUnaryOperation(+)
	DefineCqlFloatingPointUnaryOperation(-)

#undef DefineCqlFloatingPointUnaryOperation

#define DefineCqlFloatingPointCompareOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_floating_point<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::CqlUnderlyingType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::CqlUnderlyingType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_floating_point<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::CqlUnderlyingType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class T, \
		std::enable_if_t<std::is_floating_point<typename T::CqlUnderlyingType>::value, int> = 0> \
	bool operator op(const T& a, const typename T::CqlUnderlyingType& b) { \
		return a.get() op b; \
	} \
	template <class T, \
		std::enable_if_t<std::is_floating_point<typename T::CqlUnderlyingType>::value, int> = 0> \
	bool operator op(const typename T::CqlUnderlyingType& a, const T& b) { \
		return a op b.get(); \
	}

	// notice: compare floating point may not produce correct answer
	// use (a-b)<epsilon instead if needed
	DefineCqlFloatingPointCompareOperation(==)
	DefineCqlFloatingPointCompareOperation(!=)
	DefineCqlFloatingPointCompareOperation(>)
	DefineCqlFloatingPointCompareOperation(<)
	DefineCqlFloatingPointCompareOperation(>=)
	DefineCqlFloatingPointCompareOperation(<=)

#undef DefineCqlFloatingPointCompareOperation

#define DefineCqlFloatingPointCompoundAssignOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_floating_point<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::CqlUnderlyingType>::value, int> = 0> \
	TA& operator op(TA& a, const TB& b) { \
		a.get() op b.get(); \
		return a; \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_floating_point<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::CqlUnderlyingType>::value, int> = 0> \
	TA& operator op(TA& a, const TB& b) { \
		a.get() op b.get(); \
		return a; \
	} \
	template <class T, \
		std::enable_if_t<std::is_floating_point<typename T::CqlUnderlyingType>::value, int> = 0> \
	T& operator op(T& a, const typename T::CqlUnderlyingType& b) { \
		a.get() op b; \
		return a; \
	} \
	template <class T, \
		std::enable_if_t<std::is_floating_point<typename T::CqlUnderlyingType>::value, int> = 0> \
	typename T::CqlUnderlyingType& operator op(typename T::CqlUnderlyingType& a, const T& b) { \
		return a op b.get(); \
	}

	DefineCqlFloatingPointCompoundAssignOperation(+=)
	DefineCqlFloatingPointCompoundAssignOperation(-=)
	DefineCqlFloatingPointCompoundAssignOperation(*=)
	DefineCqlFloatingPointCompoundAssignOperation(/=)

#undef DefineCqlFloatingPointCompoundAssignOperation

	/** Write text representation of floating point value to stream */
	template <class T,
		std::enable_if_t<std::is_floating_point<typename T::CqlUnderlyingType>::value, int> = 0>
	std::ostream& operator<<(std::ostream& stream, const T& value) {
		stream << value.get();
		return stream;
	}
}

