#pragma once
#include <stdexcept>

// convenient macro to tell where is the line that includes this code
#define CQL_CODEINFO __FILE__ ":" __LINE__ "(" __PRETTY_FUNCTION__ ")"

namespace cql {
	/**
	 * The base class of all exceptions this library will throw.
	 * Example: throw CqlException(CQL_CODEINFO, "some error");
	 */
	class CqlException : public std::runtime_error {
	public:
		// TODO: join args
		template <class... Args>
		CqlException(const char* codeInfo, Args&&... args) :
			runtime_error(codeInfo) { }
	};
}

