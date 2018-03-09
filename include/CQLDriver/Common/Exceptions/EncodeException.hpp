#pragma once
#include "./Exception.hpp"

namespace cql {
	/**
	 * Data encode error, usually cause by incorrect client implementation
	 * Example: throw EncodeException(CQL_CODEINFO, "some error");
	 */
	class EncodeException : public Exception {
	public:
		using Exception::Exception;
	};
}

