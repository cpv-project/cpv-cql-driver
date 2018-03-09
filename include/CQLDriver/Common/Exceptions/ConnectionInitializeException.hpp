#pragma once
#include "./Exception.hpp"

namespace cql {
	/**
	 * Exception represent initialization failure of database connection
	 * Example: throw ConnectionInitializeException(CQL_CODEINFO, "some error");
	 */
	class ConnectionInitializeException : public Exception {
	public:
		using Exception::Exception;
	};
}

