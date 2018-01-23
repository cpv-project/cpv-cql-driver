#pragma once
#include "CqlException.hpp"

namespace cql {
	/**
	 * Exception represent initialization failure of database connection
	 * Example: throw CqlConnectionInitializeException(CQL_CODEINFO, "some error");
	 */
	class CqlConnectionInitializeException : public CqlException {
	public:
		using CqlException::CqlException;
	};
}

