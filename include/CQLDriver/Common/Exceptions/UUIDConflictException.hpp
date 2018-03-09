#pragma once
#include "./Exception.hpp"

namespace cql {
	/**
	 * Exception throws when two uuid is conflict
	 * Example: throw UUIDConflictException(CQL_CODEINFO, "some error");
	 */
	class UUIDConflictException : public Exception {
	public:
		using Exception::Exception;
	};
}

