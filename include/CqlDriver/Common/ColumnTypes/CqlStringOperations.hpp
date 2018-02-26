#pragma once
#include <utility>
#include <type_traits>
#include <core/sstring.hh>

namespace cql {
#define DefineCqlStringCompareOperation(op) \
	template <class TA, class TB, \
		std::enable_if_t<std::is_same< \
			typename TA::CqlUnderlyingType, seastar::sstring>::value, int> = 0, \
		std::enable_if_t<std::is_same< \
			typename TB::CqlUnderlyingType, seastar::sstring>::value, int> = 0> \
	bool operator op(const TA& a, const TB& b) { \
		return a.get() op b.get(); \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::CqlUnderlyingType, seastar::sstring>::value, int> = 0> \
	bool operator op(const T& a, const seastar::sstring& b) { \
		return a.get() op b; \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::CqlUnderlyingType, seastar::sstring>::value, int> = 0> \
	bool operator op(const T& a, const char* b) { \
		return a.get() op b; \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::CqlUnderlyingType, seastar::sstring>::value, int> = 0> \
	bool operator op(const seastar::sstring& a, const T& b) { \
		return b.get() op a; \
	} \
	template <class T, \
		std::enable_if_t<std::is_same< \
			typename T::CqlUnderlyingType, seastar::sstring>::value, int> = 0> \
	bool operator op(const char* a, const T& b) { \
		return b.get() op a; \
	}

	DefineCqlStringCompareOperation(==);
	DefineCqlStringCompareOperation(!=);

#undef DefineCqlStringCompareOperation
}

