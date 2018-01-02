#pragma once
#include <string>
#include "CqlConnectorBase.hpp"

namespace cql {
	class CqlSslConnector : public CqlConnectorBase {
	public:
		/** Constructor */
		CqlSslConnector(const std::string& pem);
	};
}

