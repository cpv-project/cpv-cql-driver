#pragma once
#include "CqlAuthenticatorBase.hpp"

namespace cql {
	/** TODO */
	class CqlAllowAllAuthenticator : public CqlAuthenticatorBase {
	public:
		/** Perform the authentication */
		seastar::future<> authenticate(CqlConnection&) const override;
	};
}

