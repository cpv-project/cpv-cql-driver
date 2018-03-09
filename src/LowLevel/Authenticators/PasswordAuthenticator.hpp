#pragma once
#include <string>
#include "./AuthenticatorBase.hpp"

namespace cql {
	/** Perform password authentication */
	class PasswordAuthenticator : public AuthenticatorBase {
	public:
		/** Perform the authentication */
		seastar::future<> authenticate(
			const seastar::lw_shared_ptr<Connection>& connection,
			const ConnectionStream& stream) const override;
	};
}

