#pragma once
#include "CqlException.hpp"

namespace cql {
	/**
	 * Exception represent there no available database connection
	 * Example: throw CqlConnectionNotAvailableException(CQL_CODEINFO, "some error");
	 */
	class CqlConnectionNotAvailableException : public CqlException {
	public:
		using CqlException::CqlException;
	};
}

