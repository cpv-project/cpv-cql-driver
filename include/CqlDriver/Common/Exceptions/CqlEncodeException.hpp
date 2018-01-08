#pragma once
#include "CqlException.hpp"

namespace cql {
	/**
	 * Data encode error, usually cause by incorrect client implementation
	 * Example: throw CqlEncodeException(CQL_CODEINFO, "some error");
	 */
	class CqlEncodeException : public CqlException {
	public:
		using CqlException::CqlException;
	};
}

