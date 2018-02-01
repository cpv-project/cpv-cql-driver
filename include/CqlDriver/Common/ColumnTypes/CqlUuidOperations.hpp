#pragma once
#include <type_traits>
#include <CqlDriver/Common/Utility/UuidUtils.hpp>

namespace cql {
#define DefineCqlUuidCompareOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_same< \
			typename TA::CqlUnderlyingType, UuidDataType>::value, int> = 0, \
		std::enable_if_t<std::is_same< \
			typename TB::CqlUnderlyingType, UuidDataType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::CqlUnderlyingType, UuidDataType>::value, int> = 0> \
	bool operator op(const T& a, const typename T::CqlUnderlyingType& b) { \
		return a.get() op b; \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::CqlUnderlyingType, UuidDataType>::value, int> = 0> \
	bool operator op(const typename T::CqlUnderlyingType& a, const T& b) { \
		return a op b.get(); \
	}

	DefineCqlUuidCompareOperation(==)
	DefineCqlUuidCompareOperation(!=)
	DefineCqlUuidCompareOperation(>=)
	DefineCqlUuidCompareOperation(>)
	DefineCqlUuidCompareOperation(<=)
	DefineCqlUuidCompareOperation(<)

#undef DefineCqlUuidCompareOperation

	/** Write text representation of uuid value to stream */
	template <class T,
		std::enable_if_t<std::is_same<
			typename T::CqlUnderlyingType, UuidDataType>::value, int> = 0>
	std::ostream& operator<<(std::ostream& stream, const T& value) {
		stream << uuidToStr(value.get());
		return stream;
	}
}

