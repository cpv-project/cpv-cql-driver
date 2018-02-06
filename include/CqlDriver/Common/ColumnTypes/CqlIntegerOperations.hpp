#pragma once
#include <utility>
#include <type_traits>

namespace cql {
#define DefineCqlIntegerBinaryOperation(op) \
	template <class TA, class TB, \
		class TResult = std::conditional_t<sizeof(TA) >= sizeof(TB), TA, TB>, \
		std::enable_if_t<std::is_integral<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::CqlUnderlyingType>::value, int> = 0> \
	TResult operator op(const TA& a, const TB& b) { \
		return TResult(a.get() op b.get()); \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<TB>::value, int> = 0> \
	TA operator op(const TA& a, const TB& b) { \
		return TA(a.get() op b); \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<TA>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::CqlUnderlyingType>::value, int> = 0> \
	TA operator op(const TA& a, const TB& b) { \
		return TA(a op b.get()); \
	}

	DefineCqlIntegerBinaryOperation(+)
	DefineCqlIntegerBinaryOperation(-)
	DefineCqlIntegerBinaryOperation(*)
	DefineCqlIntegerBinaryOperation(/)
	DefineCqlIntegerBinaryOperation(%)
	DefineCqlIntegerBinaryOperation(^)
	DefineCqlIntegerBinaryOperation(&)
	DefineCqlIntegerBinaryOperation(|)
	DefineCqlIntegerBinaryOperation(>>)
	DefineCqlIntegerBinaryOperation(<<)
	DefineCqlIntegerBinaryOperation(&&)
	DefineCqlIntegerBinaryOperation(||)

#undef DefineCqlIntegerBinaryOperation

#define DefineCqlIntegerUnaryOperation(op) \
	template <class T, \
		std::enable_if_t<std::is_integral<typename T::CqlUnderlyingType>::value, int> = 0> \
	T operator op(const T& value) { \
		return T(op value.get()); \
	}

	DefineCqlIntegerUnaryOperation(+);
	DefineCqlIntegerUnaryOperation(-);
	DefineCqlIntegerUnaryOperation(~);
	DefineCqlIntegerUnaryOperation(!);

#undef DefineCqlIntegerUnaryOperation

#define DefineCqlIntegerPrefixOperation(op) \
	template <class T, \
		std::enable_if_t<std::is_integral<typename T::CqlUnderlyingType>::value, int> = 0> \
	T& operator op(T& value) { \
		op value.get(); \
		return value; \
	} \
	template <class T, \
		std::enable_if_t<std::is_integral<typename T::CqlUnderlyingType>::value, int> = 0> \
	T operator op(T& value, int) { \
		return T(value.get() op); \
	}

	DefineCqlIntegerPrefixOperation(++);
	DefineCqlIntegerPrefixOperation(--);

#undef DefineCqlIntegerPrefixOperation

#define DefineCqlIntegerCompareOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::CqlUnderlyingType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<TB>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b; \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<TA>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::CqlUnderlyingType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a op b.get(); \
	}

	DefineCqlIntegerCompareOperation(==)
	DefineCqlIntegerCompareOperation(!=)
	DefineCqlIntegerCompareOperation(>)
	DefineCqlIntegerCompareOperation(<)
	DefineCqlIntegerCompareOperation(>=)
	DefineCqlIntegerCompareOperation(<=)

#undef DefineCqlIntegerCompareOperation

#define DefineCqlIntegerCompoundAssignOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::CqlUnderlyingType>::value, int> = 0> \
	TA& operator op(TA& a, const TB& b) { \
		a.get() op b.get(); \
		return a; \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<typename TA::CqlUnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<TB>::value, int> = 0> \
	TA& operator op(TA& a, const TB& b) { \
		a.get() op b; \
		return a; \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<TA>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::CqlUnderlyingType>::value, int> = 0> \
	TA& operator op(TA& a, const TB& b) { \
		return a op b.get(); \
	}

	DefineCqlIntegerCompoundAssignOperation(+=)
	DefineCqlIntegerCompoundAssignOperation(-=)
	DefineCqlIntegerCompoundAssignOperation(*=)
	DefineCqlIntegerCompoundAssignOperation(/=)
	DefineCqlIntegerCompoundAssignOperation(%=)
	DefineCqlIntegerCompoundAssignOperation(^=)
	DefineCqlIntegerCompoundAssignOperation(&=)
	DefineCqlIntegerCompoundAssignOperation(|=)
	DefineCqlIntegerCompoundAssignOperation(>>=)
	DefineCqlIntegerCompoundAssignOperation(<<=)

#undef DefineCqlIntegerCompoundAssignOperation

	/** Write text representation of integer value to stream */
	template <class T,
		std::enable_if_t<std::is_integral<typename T::CqlUnderlyingType>::value, int> = 0>
	std::ostream& operator<<(std::ostream& stream, const T& value) {
		stream << value.get();
		return stream;
	}
}

