#include "CqlSessionConfigurationData.hpp"
#include "../LowLevel/Connectors/CqlTcpConnector.hpp"
#include "../LowLevel/Authenticators/CqlEmptyAuthenticator.hpp"

namespace cql {
	/** Constructor */
	CqlSessionConfigurationData::CqlSessionConfigurationData() :
		serverAddresses(),
		connector(seastar::make_shared<CqlTcpConnector>()),
		authenticator(seastar::make_shared<CqlEmptyAuthenticator>()),
		maxPoolSize(100),
		autoDiscoverServerAddress(false) { }
}

