#pragma once
#include "CqlException.hpp"

namespace cql {
	/**
	 * Logic error, usually mean there something wrong in the code
	 * Example: throw CqlLogicException(CQL_CODEINFO, "some error");
	 */
	class CqlLogicException : public CqlException {
	public:
		using CqlException::CqlException;
	};
}

