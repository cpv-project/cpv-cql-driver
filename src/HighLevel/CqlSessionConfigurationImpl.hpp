#pragma once
#include <vector>
#include <string>
#include <net/socket_defs.hh>
#include <core/sharded.hh>
#include "../LowLevel/Connectors/CqlConnectorBase.hpp"
#include "../LowLevel/Authenticators/CqlAuthenticatorBase.hpp"

namespace cql {
	class CqlSessionConfigurationImpl {
	public:
		CqlSessionConfigurationImpl();

		std::vector<seastar::socket_address> serverAddresses;
		seastar::shared_ptr<CqlConnectorBase> connector;
		seastar::shared_ptr<CqlAuthenticatorBase> authenticator;
		std::size_t maxPoolSize;
		bool autoDiscoverServerAddress;
	};
}

