#pragma once
#include <stdexcept>
#include "../Utility/StringUtils.hpp"

// convenient macro to tell where is the line that includes this code
#define CQL_CODEINFO cql::joinString("", "[", __FILE__, ":", __LINE__, ":", __PRETTY_FUNCTION__, "]")

namespace cql {
	/**
	 * The base class of all exceptions this library will throw.
	 * Example: throw CqlException(CQL_CODEINFO, "some error");
	 */
	class CqlException : public std::runtime_error {
	public:
		/** Constructor */
		template <class... Args>
		CqlException(const std::string& codeInfo, Args&&... args) :
			runtime_error(joinString(" ", codeInfo, std::forward<Args>(args)...)) { }
	};
}

