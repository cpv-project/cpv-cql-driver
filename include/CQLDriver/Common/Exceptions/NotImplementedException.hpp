#pragma once
#include "./Exception.hpp"

namespace cql {
	/**
	 * Exception represent feature not yet implemented
	 * Example: throw NotImplementedException(CQL_CODEINFO, "some error");
	 */
	class NotImplementedException : public Exception {
	public:
		using Exception::Exception;
	};
}

