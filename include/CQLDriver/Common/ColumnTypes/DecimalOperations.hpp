#pragma once
#include <iomanip>
#include <utility>
#include "../Utility/MathUtils.hpp"
#include "./Int.hpp"
#include "./VarInt.hpp"

namespace cql {
	using DecimalDataType = std::pair<Int, VarInt>;

#define CQLDefineDecimalBinaryOperation(op, opAssign) \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0> \
	T operator op(const T& a, const T& b) { \
		auto result = T(a); \
		result opAssign b; \
		return result; \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0> \
	T operator op(const T& a, const typename T::UnderlyingType& b) { \
		return a op T(b); \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0> \
	T operator op(const typename T::UnderlyingType& a, const T& b) { \
		return T(a) op b; \
	}

	CQLDefineDecimalBinaryOperation(+, +=)
	CQLDefineDecimalBinaryOperation(-, -=)
	CQLDefineDecimalBinaryOperation(*, *=)
	CQLDefineDecimalBinaryOperation(/, /=)

#undef CQLDefineDecimalBinaryOperation 

#define CQLDefineDecimalUnaryOperation(op) \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0> \
	T operator op(const T& value) { \
		return T(value.get().first, op value.get().second); \
	}

	CQLDefineDecimalUnaryOperation(+)
	CQLDefineDecimalUnaryOperation(-)

#undef CQLDefineDecimalUnaryOperation

#define CQLDefineDecimalCompoundAssignAddSubImplementation(op) \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0> \
	T& operator op(T& a, const T& b) { \
		auto scaleDiff = a.get().first.get() - b.get().first.get(); \
		if (scaleDiff > 0) { \
			a.get().second op (b.get().second * powInteger(10ULL, scaleDiff)); \
		} else if (scaleDiff < 0) { \
			a.get().first -= scaleDiff; \
			a.get().second *= powInteger(10ULL, -scaleDiff); \
			a.get().second op b.get().second; \
		} else { \
			a.get().second op b.get().second; \
		} \
		return a; \
	}

	CQLDefineDecimalCompoundAssignAddSubImplementation(+=)
	CQLDefineDecimalCompoundAssignAddSubImplementation(-=)

#undef CQLDefineDecimalCompoundAssignAddSubImplementation

#define CQLDefineDecimalCompoundAssignMulImplementation(op) \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0> \
	T& operator op(T& a, const T& b) { \
		a.get().first += b.get().first; \
		a.get().second op b.get().second; \
		return a; \
	}

	CQLDefineDecimalCompoundAssignMulImplementation(*=)

#undef CQLDefineDecimalCompoundAssignMulImplementation

#define CQLDefineDecimalCompoundAssignDivImplementation(op) \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0> \
	T& operator op(T& a, const T& b) { \
		auto bScale = b.get().first.get(); \
		if (bScale > 0) { \
			a.get().second *= powInteger(10ULL, bScale); \
		} else if (bScale < 0) { \
			a.get().second /= powInteger(10ULL, -bScale); \
		} \
		/* scale of a unchanged */ \
		a.get().second op b.get().second; \
		return a; \
	}

	CQLDefineDecimalCompoundAssignDivImplementation(/=)

#undef CQLDefineDecimalCompoundAssignDivImplementation

#define CQLDefineDecimalCompoundAssignOverload(op) \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0> \
	T& operator op(T& a, const typename T::UnderlyingType& b) { \
		a op T(b); \
		return a; \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0> \
	typename T::UnderlyingType& operator op(typename T::UnderlyingType& a, const T& b) { \
		T result(a); \
		result op b; \
		a = result.get(); \
		return a; \
	}

	CQLDefineDecimalCompoundAssignOverload(+=)
	CQLDefineDecimalCompoundAssignOverload(-=)
	CQLDefineDecimalCompoundAssignOverload(*=)
	CQLDefineDecimalCompoundAssignOverload(/=)

#undef CQLDefineDecimalCompoundAssignOverload

#define CQLDefineDecimalCompareOperation(op) \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0> \
	bool operator op(const T& a, const T& b) { \
		auto scaleDiff = a.get().first.get() - b.get().first.get(); \
		if (scaleDiff > 0) { \
			return a.get().second op (b.get().second * powInteger(10ULL, scaleDiff)); \
		} else if (scaleDiff < 0) { \
			return (a.get().second * powInteger(10ULL, -scaleDiff)) op b.get().second; \
		} else { \
			return a.get().second op b.get().second; \
		} \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0> \
	bool operator op(const T& a, const typename T::UnderlyingType& b) { \
		return a op T(b); \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0> \
	bool operator op(const typename T::UnderlyingType& a, const T& b) { \
		return T(a) op b; \
	}

	CQLDefineDecimalCompareOperation(==)
	CQLDefineDecimalCompareOperation(!=)
	CQLDefineDecimalCompareOperation(>)
	CQLDefineDecimalCompareOperation(<)
	CQLDefineDecimalCompareOperation(>=)
	CQLDefineDecimalCompareOperation(<=)

#undef CQLDefineDecimalCompareOperation

	/** Write text representation of decimal value to stream */
	template <class T,
		std::enable_if_t<std::is_same<
			typename T::UnderlyingType, DecimalDataType>::value, int> = 0>
	std::ostream& operator<<(std::ostream& stream, const T& value) {
		auto scale = value.get().first.get();
		if (scale == 0) {
			stream << value.get().second;
		} else if (scale > 0) {
			auto unscaled = value.get().second.get();
			auto unscaledAbs = std::abs(unscaled);
			auto boundary = powInteger(10ULL, scale);
			auto sign = unscaled >= 0 ? "" : "-";
			stream << sign << (unscaledAbs / boundary) << '.' <<
				std::setfill('0') << std::setw(scale) << (unscaledAbs % boundary);
		} else { // scale < 0
			auto unscaled = value.get().second;
			stream << unscaled << std::setfill('0') << std::setw(-scale) << '0';
		}
		return stream;
	}
}

