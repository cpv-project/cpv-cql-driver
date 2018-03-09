#pragma once
#include "./AuthenticatorBase.hpp"

namespace cql {
	/** Perform no authentication, just wait for READY message */
	class AllowAllAuthenticator : public AuthenticatorBase {
	public:
		/** Perform the authentication */
		seastar::future<> authenticate(
			const seastar::lw_shared_ptr<Connection>& connection,
			const ConnectionStream& stream) const override;
	};
}

