#pragma once
#include <stdexcept>
#include "../Utility/StringUtils.hpp"
#include "../Utility/CodeInfo.hpp"

namespace cql {
	/**
	 * The base class of all exceptions this library will throw.
	 * Example: throw CqlException(CQL_CODEINFO, "some error");
	 */
	class CqlException : public std::runtime_error {
	public:
		/** Constructor */
		template <class... Args>
		CqlException(CodeInfo&& codeInfo, Args&&... args) :
			runtime_error(joinString(" ", std::move(codeInfo).str(), std::forward<Args>(args)...)) { }
	};
}

