#pragma once
#include "CqlException.hpp"

namespace cql {
	/**
	 * Network related exception like connect, send or receive error
	 * Example: throw CqlNetworkException(CQL_CODEINFO, "some error");
	 */
	class CqlNetworkException : public CqlException {
	public:
		using CqlException::CqlException;
	};
}

