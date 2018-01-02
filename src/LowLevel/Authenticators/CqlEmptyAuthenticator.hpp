#pragma once
#include "CqlAuthenticatorBase.hpp"

namespace cql {
	/** TODO */
	class CqlEmptyAuthenticator : public CqlAuthenticatorBase {
	public:
		/** Perform the authentication */
		seastar::future<> authenticate(CqlConnection&) const override;
	};
}

