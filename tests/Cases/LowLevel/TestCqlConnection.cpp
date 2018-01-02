#include <LowLevel/CqlConnection.hpp>
#include <LowLevel/Connectors/CqlTcpConnector.hpp>
#include <LowLevel/Connectors/CqlSslConnector.hpp>
#include <LowLevel/Authenticators/CqlEmptyAuthenticator.hpp>
#include <LowLevel/Authenticators/CqlPasswordAuthenticator.hpp>
#include <gtest/gtest.h>
#include "../../TestUtility/AppTest.hpp"

namespace { static char ProgramName[] = "TestConnection"; }

TEST(TestConnection, waitForReadySimple) {
	cql::CqlConnection connection(
		seastar::make_ipv4_address({ DB_SIMPLE_IP, DB_SIMPLE_PORT }),
		seastar::make_shared<cql::CqlTcpConnector>(),
		seastar::make_shared<cql::CqlEmptyAuthenticator>());
	cql::runAppTest([&connection] {
		return connection.ready();
	});
}

TEST(TestConnection, waitForReadySsl) {
	
}

