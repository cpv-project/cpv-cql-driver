#pragma once
#include "./Exception.hpp"

namespace cql {
	/**
	 * Exception represent authentication failure of database connection
	 * Example: throw AuthenticateException(CQL_CODEINFO, "some error");
	 */
	class AuthenticateException : public Exception {
	public:
		using Exception::Exception;
	};
}

