#include "CqlSessionConfigurationImpl.hpp"
#include "../LowLevel/Connectors/CqlTcpConnector.hpp"
#include "../LowLevel/Authenticators/CqlEmptyAuthenticator.hpp"

namespace cql {
	/** Constructor */
	CqlSessionConfigurationImpl::CqlSessionConfigurationImpl() :
		serverAddresses(),
		connector(seastar::make_shared<CqlTcpConnector>()),
		authenticator(seastar::make_shared<CqlEmptyAuthenticator>()),
		maxPoolSize(100),
		autoDiscoverServerAddress(false) { }
}

