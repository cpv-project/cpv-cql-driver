#pragma once
#include <utility>
#include <type_traits>
#include <string>

namespace cql {
#define CQLDefineStringCompareOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_same< \
			typename TA::UnderlyingType, std::string>::value, int> = 0, \
		std::enable_if_t<std::is_same< \
			typename TB::UnderlyingType, std::string>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, std::string>::value, int> = 0> \
	bool operator op(const T& a, const std::string& b) { \
		return a.get() op b; \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, std::string>::value, int> = 0> \
	bool operator op(const T& a, const char* b) { \
		return a.get() op b; \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, std::string>::value, int> = 0> \
	bool operator op(const std::string& a, const T& b) { \
		return b.get() op a; \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::UnderlyingType, std::string>::value, int> = 0> \
	bool operator op(const char* a, const T& b) { \
		return b.get() op a; \
	}

	CQLDefineStringCompareOperation(==)
	CQLDefineStringCompareOperation(!=)

#undef CQLDefineStringCompareOperation
}

