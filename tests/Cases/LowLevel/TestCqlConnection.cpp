#include <CqlDriver/Common/Exceptions/CqlNetworkException.hpp>
#include <LowLevel/CqlConnection.hpp>
#include <TestUtility/GTestUtils.hpp>
#include <core/sleep.hh>

TEST_FUTURE(TestConnection, waitForReadySimple) {
	auto connection = seastar::make_shared<cql::CqlConnection>(
		seastar::make_shared<cql::CqlSessionConfiguration>(),
		seastar::make_shared<cql::CqlNodeConfiguration>(
			cql::CqlNodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)));
	return connection->ready();
}

TEST_FUTURE(TestConnection, waitForReadySsl) {
	auto connection = seastar::make_shared<cql::CqlConnection>(
		seastar::make_shared<cql::CqlSessionConfiguration>(),
		seastar::make_shared<cql::CqlNodeConfiguration>(
			cql::CqlNodeConfiguration()
				.setAddress(DB_SSL_1_IP, DB_SSL_1_PORT)
				.setUseSsl(true)
				.setUseCompression(true)));
	return connection->ready();
}

TEST_FUTURE(TestConnection, waitForReadyFailed) {
	auto connection = seastar::make_shared<cql::CqlConnection>(
		seastar::make_shared<cql::CqlSessionConfiguration>(),
		seastar::make_shared<cql::CqlNodeConfiguration>(
			cql::CqlNodeConfiguration()
				.setAddress("host.not.exist", 123)));
	return connection->ready().then_wrapped([] (auto&& f) {
		ASSERT_THROWS(cql::CqlNetworkException, f.get());
	});
}

