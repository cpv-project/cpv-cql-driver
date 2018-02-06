#pragma once
#include <cstdint>

namespace cql {
	/** The fast integer pow function */
	template <class T>
	T powInteger(T base, std::size_t exponent) {
		T result(1);
		while (exponent > 0) {
			if (exponent & 1) {
				result *= base;
			}
			base *= base;
			exponent >>= 1;
		}
		return result;
	}
}

