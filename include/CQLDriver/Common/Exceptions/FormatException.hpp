#pragma once
#include "./Exception.hpp"

namespace cql {
	/**
	 * Data format error, usually cause by library user
	 * Example: throw FormatException(CQL_CODEINFO, "some error");
	 */
	class FormatException : public Exception {
	public:
		using Exception::Exception;
	};
}

