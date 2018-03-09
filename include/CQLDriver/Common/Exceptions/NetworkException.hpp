#pragma once
#include "./Exception.hpp"

namespace cql {
	/**
	 * Network related exception like connect, send or receive error
	 * Example: throw NetworkException(CQL_CODEINFO, "some error");
	 */
	class NetworkException : public Exception {
	public:
		using Exception::Exception;
	};
}

