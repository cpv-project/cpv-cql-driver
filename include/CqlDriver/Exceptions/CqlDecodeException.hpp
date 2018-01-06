#pragma once
#include "CqlException.hpp"

namespace cql {
	/**
	 * Data decode error, usually cause by incorrect client or server implementation
	 * Example: throw CqlDecodeException(CQL_CODEINFO, "some error");
	 */
	class CqlDecodeException : public CqlException {
	public:
		using CqlException::CqlException;
	};
}

