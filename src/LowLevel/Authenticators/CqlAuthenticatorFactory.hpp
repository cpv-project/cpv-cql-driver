#pragma once
#include <core/shared_ptr.hh>
#include <CqlDriver/Common/CqlNodeConfiguration.hpp>
#include "CqlAuthenticatorBase.hpp"

namespace cql {
	namespace CqlAuthenticatorFactory {
		/** Get a suitable authenticator for specific node */
		seastar::shared_ptr<CqlAuthenticatorBase> getAuthenticator(
			const CqlNodeConfiguration& nodeConfiguration);
	}
}

