#pragma once
#include "CqlException.hpp"

namespace cql {
	/**
	 * Exception represent authentication failure of database connection
	 * Example: throw CqlAuthenticateException(CQL_CODEINFO, "some error");
	 */
	class CqlAuthenticateException : public CqlException {
	public:
		using CqlException::CqlException;
	};
}

