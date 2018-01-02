#pragma once
#include <string>
#include "CqlAuthenticatorBase.hpp"

namespace cql {
	class CqlPasswordAuthenticator : public CqlAuthenticatorBase {
	public:
		/** Constructor */
		CqlPasswordAuthenticator(const std::string& username, const std::string& password);
	};
}

