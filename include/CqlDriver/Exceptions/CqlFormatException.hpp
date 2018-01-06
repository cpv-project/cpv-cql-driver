#pragma once
#include "CqlException.hpp"

namespace cql {
	/**
	 * Data format error, usually cause by library user
	 * Example: throw CqlFormatException(CQL_CODEINFO, "some error");
	 */
	class CqlFormatException : public CqlException {
	public:
		using CqlException::CqlException;
	};
}

