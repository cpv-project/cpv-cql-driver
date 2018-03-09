#pragma once
#include "./Exception.hpp"

namespace cql {
	/**
	 * Data decode error, usually cause by incorrect client or server implementation
	 * Example: throw DecodeException(CQL_CODEINFO, "some error");
	 */
	class DecodeException : public Exception {
	public:
		using Exception::Exception;
	};
}

