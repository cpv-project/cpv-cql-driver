#pragma once
#include <utility>
#include <type_traits>

namespace cql {
#define CQLDefineIntegerBinaryOperation(op) \
	template <class TA, class TB, \
		class TResult = std::conditional_t<sizeof(TA) >= sizeof(TB), TA, TB>, \
		std::enable_if_t<std::is_integral<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::UnderlyingType>::value, int> = 0> \
	TResult operator op(const TA& a, const TB& b) { \
		return TResult(a.get() op b.get()); \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<TB>::value, int> = 0> \
	TA operator op(const TA& a, const TB& b) { \
		return TA(a.get() op b); \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<TA>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::UnderlyingType>::value, int> = 0> \
	TA operator op(const TA& a, const TB& b) { \
		return TA(a op b.get()); \
	}

	CQLDefineIntegerBinaryOperation(+)
	CQLDefineIntegerBinaryOperation(-)
	CQLDefineIntegerBinaryOperation(*)
	CQLDefineIntegerBinaryOperation(/)
	CQLDefineIntegerBinaryOperation(%)
	CQLDefineIntegerBinaryOperation(^)
	CQLDefineIntegerBinaryOperation(&)
	CQLDefineIntegerBinaryOperation(|)
	CQLDefineIntegerBinaryOperation(>>)
	CQLDefineIntegerBinaryOperation(<<)
	CQLDefineIntegerBinaryOperation(&&)
	CQLDefineIntegerBinaryOperation(||)

#undef CQLDefineIntegerBinaryOperation

#define CQLDefineIntegerUnaryOperation(op) \
	template <class T, \
		std::enable_if_t<std::is_integral<typename T::UnderlyingType>::value, int> = 0> \
	T operator op(const T& value) { \
		return T(op value.get()); \
	}

	CQLDefineIntegerUnaryOperation(+)
	CQLDefineIntegerUnaryOperation(-)
	CQLDefineIntegerUnaryOperation(~)
	CQLDefineIntegerUnaryOperation(!)

#undef CQLDefineIntegerUnaryOperation

#define CQLDefineIntegerPrefixOperation(op) \
	template <class T, \
		std::enable_if_t<std::is_integral<typename T::UnderlyingType>::value, int> = 0> \
	T& operator op(T& value) { \
		op value.get(); \
		return value; \
	} \
	template <class T, \
		std::enable_if_t<std::is_integral<typename T::UnderlyingType>::value, int> = 0> \
	T operator op(T& value, int) { \
		return T(value.get() op); \
	}

	CQLDefineIntegerPrefixOperation(++)
	CQLDefineIntegerPrefixOperation(--)

#undef CQLDefineIntegerPrefixOperation

#define CQLDefineIntegerCompareOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::UnderlyingType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<TB>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b; \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<TA>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::UnderlyingType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a op b.get(); \
	}

	CQLDefineIntegerCompareOperation(==)
	CQLDefineIntegerCompareOperation(!=)
	CQLDefineIntegerCompareOperation(>)
	CQLDefineIntegerCompareOperation(<)
	CQLDefineIntegerCompareOperation(>=)
	CQLDefineIntegerCompareOperation(<=)

#undef CQLDefineIntegerCompareOperation

#define CQLDefineIntegerCompoundAssignOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::UnderlyingType>::value, int> = 0> \
	TA& operator op(TA& a, const TB& b) { \
		a.get() op b.get(); \
		return a; \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<TB>::value, int> = 0> \
	TA& operator op(TA& a, const TB& b) { \
		a.get() op b; \
		return a; \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<TA>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::UnderlyingType>::value, int> = 0> \
	TA& operator op(TA& a, const TB& b) { \
		return a op b.get(); \
	}

	CQLDefineIntegerCompoundAssignOperation(+=)
	CQLDefineIntegerCompoundAssignOperation(-=)
	CQLDefineIntegerCompoundAssignOperation(*=)
	CQLDefineIntegerCompoundAssignOperation(/=)
	CQLDefineIntegerCompoundAssignOperation(%=)
	CQLDefineIntegerCompoundAssignOperation(^=)
	CQLDefineIntegerCompoundAssignOperation(&=)
	CQLDefineIntegerCompoundAssignOperation(|=)
	CQLDefineIntegerCompoundAssignOperation(>>=)
	CQLDefineIntegerCompoundAssignOperation(<<=)

#undef CQLDefineIntegerCompoundAssignOperation

	/** Write text representation of integer value to stream */
	template <class T,
		std::enable_if_t<std::is_integral<typename T::UnderlyingType>::value, int> = 0>
	std::ostream& operator<<(std::ostream& stream, const T& value) {
		stream << value.get();
		return stream;
	}
}

