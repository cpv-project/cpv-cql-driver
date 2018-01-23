#pragma once
#include "CqlAuthenticatorBase.hpp"

namespace cql {
	/** Perform no authentication, just wait for READY message */
	class CqlAllowAllAuthenticator : public CqlAuthenticatorBase {
	public:
		/** Perform the authentication */
		seastar::future<> authenticate(
			const seastar::shared_ptr<CqlConnection>& connection,
			const CqlConnectionStream& stream) const override;
	};
}

