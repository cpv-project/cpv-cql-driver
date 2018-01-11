#pragma once
#include <string>
#include "CqlAuthenticatorBase.hpp"

namespace cql {
	/** TODO */
	class CqlPasswordAuthenticator : public CqlAuthenticatorBase {
	public:
		/** Perform the authentication */
		seastar::future<> authenticate(CqlConnection&) const override;
	};
}

