#pragma once
#include "CqlException.hpp"

namespace cql {
	/**
	 * Exception throws when two uuid is conflict
	 * Example: throw CqlUuidConflictException(CQL_CODEINFO, "some error");
	 */
	class CqlUuidConflictException : public CqlException {
	public:
		using CqlException::CqlException;
	};
}

