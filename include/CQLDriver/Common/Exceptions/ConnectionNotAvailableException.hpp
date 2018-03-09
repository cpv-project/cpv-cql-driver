#pragma once
#include "./Exception.hpp"

namespace cql {
	/**
	 * Exception represent there no available database connection
	 * Example: throw ConnectionNotAvailableException(CQL_CODEINFO, "some error");
	 */
	class ConnectionNotAvailableException : public Exception {
	public:
		using Exception::Exception;
	};
}

