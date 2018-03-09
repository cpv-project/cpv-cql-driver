#pragma once
#include <type_traits>
#include "../Utility/UUIDUtils.hpp"

namespace cql {
#define CQLDefineUUIDCompareOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_same< \
			typename TA::UnderlyingType, UUIDDataType>::value, int> = 0, \
		std::enable_if_t<std::is_same< \
			typename TB::UnderlyingType, UUIDDataType>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, UUIDDataType>::value, int> = 0> \
	bool operator op(const T& a, const typename T::UnderlyingType& b) { \
		return a.get() op b; \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, UUIDDataType>::value, int> = 0> \
	bool operator op(const typename T::UnderlyingType& a, const T& b) { \
		return a op b.get(); \
	}

	CQLDefineUUIDCompareOperation(==)
	CQLDefineUUIDCompareOperation(!=)
	CQLDefineUUIDCompareOperation(>=)
	CQLDefineUUIDCompareOperation(>)
	CQLDefineUUIDCompareOperation(<=)
	CQLDefineUUIDCompareOperation(<)

#undef CQLDefineUUIDCompareOperation

	/** Write text representation of uuid value to stream */
	template <class T,
		std::enable_if_t<std::is_same<
			typename T::UnderlyingType, UUIDDataType>::value, int> = 0>
	std::ostream& operator<<(std::ostream& stream, const T& value) {
		stream << uuidToStr(value.get());
		return stream;
	}
}

