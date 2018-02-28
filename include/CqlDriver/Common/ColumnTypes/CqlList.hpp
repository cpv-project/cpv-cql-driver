#pragma once
#include <vector>

namespace cql {
	template <class T>
	class CqlList {
	public:
		using CqlUnderlyingType = std::vector<T>;

	private:
	};
}

