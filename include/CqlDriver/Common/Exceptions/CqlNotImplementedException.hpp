#pragma once
#include "CqlException.hpp"

namespace cql {
	/**
	 * Exception represent feature not yet implemented
	 * Example: throw CqlNotImplementedException(CQL_CODEINFO, "some error");
	 */
	class CqlNotImplementedException : public CqlException {
	public:
		using CqlException::CqlException;
	};
}

