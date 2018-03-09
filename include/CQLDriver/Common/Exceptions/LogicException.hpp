#pragma once
#include "./Exception.hpp"

namespace cql {
	/**
	 * Logic error, usually mean there something wrong in the code
	 * Example: throw LogicException(CQL_CODEINFO, "some error");
	 */
	class LogicException : public Exception {
	public:
		using Exception::Exception;
	};
}

