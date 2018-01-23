#pragma once
#include <string>
#include "CqlAuthenticatorBase.hpp"

namespace cql {
	/** Perform password authentication */
	class CqlPasswordAuthenticator : public CqlAuthenticatorBase {
	public:
		/** Perform the authentication */
		seastar::future<> authenticate(
			const seastar::shared_ptr<CqlConnection>& connection,
			const CqlConnectionStream& stream) const override;
	};
}

