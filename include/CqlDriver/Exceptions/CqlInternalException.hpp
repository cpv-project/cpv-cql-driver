#pragma once
#include "CqlException.hpp"

namespace cql {
	/**
	 * Internal exception that only throws when something wrong in client or server implementation
	 * Example: throw CqlInternalException(CQL_CODEINFO, "some error");
	 */
	class CqlInternalException : public CqlException {
	public:
		using CqlException::CqlException;
	};
}

