#pragma once
#include <utility>
#include <type_traits>

namespace cql {
#define CQLDefineFloatingPointBinaryOperation(op) \
	template <class TA, class TB, \
		class TResult = std::conditional_t<sizeof(TA) >= sizeof(TB), TA, TB>, \
		std::enable_if_t<std::is_floating_point<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::UnderlyingType>::value, int> = 0> \
	TResult operator op(const TA& a, const TB& b) { \
		return TResult(a.get() op b.get()); \
	} \
	template <class TA, class TB, \
		class TResult = TA, \
		std::enable_if_t<std::is_floating_point<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::UnderlyingType>::value, int> = 0> \
	TResult operator op(const TA& a, const TB& b) { \
		return TResult(a.get() op b.get()); \
	} \
	template <class TA, class TB, \
		class TResult = TB, \
		std::enable_if_t<std::is_integral<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::UnderlyingType>::value, int> = 0> \
	TResult operator op(const TA& a, const TB& b) { \
		return TResult(a.get() op b.get()); \
	} \
	template <class TA, class TB, \
		class TResult = TA, \
		std::enable_if_t<std::is_floating_point<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_arithmetic<TB>::value, int> = 0> \
	TResult operator op(const TA& a, const TB& b) { \
		return TResult(a.get() op b); \
	} \
	template <class TA, class TB, \
		class TResult = TB, \
		std::enable_if_t<std::is_arithmetic<TA>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::UnderlyingType>::value, int> = 0> \
	TResult operator op(const TA& a, const TB& b) { \
		return TResult(a op b.get()); \
	}

	CQLDefineFloatingPointBinaryOperation(+)
	CQLDefineFloatingPointBinaryOperation(-)
	CQLDefineFloatingPointBinaryOperation(*)
	CQLDefineFloatingPointBinaryOperation(/)

#undef CQLDefineFloatingPointBinaryOperation

#define CQLDefineFloatingPointUnaryOperation(op) \
	template <class T, \
		std::enable_if_t<std::is_floating_point<typename T::UnderlyingType>::value, int> = 0> \
	T operator op(const T& value) { \
		return T(op value.get()); \
	}

	CQLDefineFloatingPointUnaryOperation(+)
	CQLDefineFloatingPointUnaryOperation(-)

#undef CQLDefineFloatingPointUnaryOperation

#define CQLDefineFloatingPointCompareOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_floating_point<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::UnderlyingType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_integral<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::UnderlyingType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_floating_point<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::UnderlyingType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_floating_point<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_arithmetic<TB>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b; \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_arithmetic<TA>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::UnderlyingType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a op b.get(); \
	}

	// notice: compare floating point may not produce correct answer
	// use (a-b)<epsilon instead if needed
	CQLDefineFloatingPointCompareOperation(==)
	CQLDefineFloatingPointCompareOperation(!=)
	CQLDefineFloatingPointCompareOperation(>)
	CQLDefineFloatingPointCompareOperation(<)
	CQLDefineFloatingPointCompareOperation(>=)
	CQLDefineFloatingPointCompareOperation(<=)

#undef CQLDefineFloatingPointCompareOperation

#define CQLDefineFloatingPointCompoundAssignOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_floating_point<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::UnderlyingType>::value, int> = 0> \
	TA& operator op(TA& a, const TB& b) { \
		a.get() op b.get(); \
		return a; \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_floating_point<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_integral<typename TB::UnderlyingType>::value, int> = 0> \
	TA& operator op(TA& a, const TB& b) { \
		a.get() op b.get(); \
		return a; \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_floating_point<typename TA::UnderlyingType>::value, int> = 0, \
		std::enable_if_t<std::is_arithmetic<TB>::value, int> = 0> \
	TA& operator op(TA& a, const TB& b) { \
		a.get() op b; \
		return a; \
	} \
	template <class TA, class TB, \
		std::enable_if_t<std::is_arithmetic<TA>::value, int> = 0, \
		std::enable_if_t<std::is_floating_point<typename TB::UnderlyingType>::value, int> = 0> \
	TA& operator op(TA& a, const TB& b) { \
		return a op b.get(); \
	}

	CQLDefineFloatingPointCompoundAssignOperation(+=)
	CQLDefineFloatingPointCompoundAssignOperation(-=)
	CQLDefineFloatingPointCompoundAssignOperation(*=)
	CQLDefineFloatingPointCompoundAssignOperation(/=)

#undef CQLDefineFloatingPointCompoundAssignOperation

	/** Write text representation of floating point value to stream */
	template <class T,
		std::enable_if_t<std::is_floating_point<typename T::UnderlyingType>::value, int> = 0>
	std::ostream& operator<<(std::ostream& stream, const T& value) {
		stream << value.get();
		return stream;
	}
}

