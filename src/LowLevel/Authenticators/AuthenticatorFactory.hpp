#pragma once
#include <core/shared_ptr.hh>
#include <CQLDriver/Common/NodeConfiguration.hpp>
#include "./AuthenticatorBase.hpp"

namespace cql {
	namespace AuthenticatorFactory {
		/** Get a suitable authenticator for specific node */
		seastar::shared_ptr<AuthenticatorBase> getAuthenticator(
			const NodeConfiguration& nodeConfiguration);
	}
}

