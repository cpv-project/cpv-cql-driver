#include <LowLevel/CqlConnection.hpp>
#include <LowLevel/Connectors/CqlTcpConnector.hpp>
#include <LowLevel/Connectors/CqlSslConnector.hpp>
#include <LowLevel/Authenticators/CqlAllowAllAuthenticator.hpp>
#include <LowLevel/Authenticators/CqlPasswordAuthenticator.hpp>
#include <TestUtility/GTestUtils.hpp>
#include <core/sleep.hh>

TEST_FUTURE(TestConnection, waitForReadySimple) {
	auto connection = seastar::make_shared<cql::CqlConnection>(
		seastar::make_ipv4_address({ DB_SIMPLE_IP, DB_SIMPLE_PORT }),
		seastar::make_shared<cql::CqlTcpConnector>(),
		seastar::make_shared<cql::CqlAllowAllAuthenticator>());
	return connection->ready();
}

TEST_FUTURE(TestConnection, waitForReadySsl) {
	auto connection = seastar::make_shared<cql::CqlConnection>(
		seastar::make_ipv4_address({ DB_SSL_1_IP, DB_SSL_1_PORT }),
		seastar::make_shared<cql::CqlSslConnector>(),
		seastar::make_shared<cql::CqlAllowAllAuthenticator>());
	return connection->ready();
}

