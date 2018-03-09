#pragma once
#include <stdexcept>
#include "../Utility/StringUtils.hpp"
#include "../Utility/CodeInfo.hpp"

namespace cql {
	/**
	 * The base class of all exceptions this library will throw.
	 * Example: throw Exception(CQL_CODEINFO, "some error");
	 */
	class Exception : public std::runtime_error {
	public:
		/** Constructor */
		template <class... Args>
		Exception(CodeInfo&& codeInfo, Args&&... args) :
			runtime_error(joinString(" ", std::move(codeInfo).str(), std::forward<Args>(args)...)) { }
	};
}

