#pragma once
#include "./Exception.hpp"

namespace cql {
	/**
	 * Common error returns by server side
	 * Example: throw ResponseErrorException(CQL_CODEINFO, "some error");
	 */
	class ResponseErrorException : public Exception {
	public:
		using Exception::Exception;
	};
}

