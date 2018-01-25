#include <CqlDriver/Common/Exceptions/CqlConnectionInitializeException.hpp>
#include <LowLevel/CqlConnection.hpp>
#include <TestUtility/GTestUtils.hpp>
#include <core/sleep.hh>

TEST_FUTURE(TestConnection, waitForReadySimple) {
	auto connection = seastar::make_lw_shared<cql::CqlConnection>(
		seastar::make_lw_shared<cql::CqlSessionConfiguration>(),
		seastar::make_lw_shared<cql::CqlNodeConfiguration>(
			cql::CqlNodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)));
	return connection->ready();
}

TEST_FUTURE(TestConnection, waitForReadySsl) {
	auto connection = seastar::make_lw_shared<cql::CqlConnection>(
		seastar::make_lw_shared<cql::CqlSessionConfiguration>(),
		seastar::make_lw_shared<cql::CqlNodeConfiguration>(
			cql::CqlNodeConfiguration()
				.setAddress(DB_SSL_1_IP, DB_SSL_1_PORT)
				.setUseSsl(true)
				.setUseCompression(true)
				.setPasswordAuthentication("cassandra", "cassandra")));
	return connection->ready();
}

TEST_FUTURE(TestConnection, waitForReadyConnectFailed) {
	auto connection = seastar::make_lw_shared<cql::CqlConnection>(
		seastar::make_lw_shared<cql::CqlSessionConfiguration>(),
		seastar::make_lw_shared<cql::CqlNodeConfiguration>(
			cql::CqlNodeConfiguration()
				.setAddress("host.not.exist", 123)));
	return connection->ready().then_wrapped([] (auto&& f) {
		ASSERT_THROWS_CONTAINS(
			cql::CqlConnectionInitializeException, f.get(),
			"connect to host.not.exist failed:");
	});
}

TEST_FUTURE(TestConnection, waitForReadyAllowAllAuthenticateFailed) {
	auto connection = seastar::make_lw_shared<cql::CqlConnection>(
		seastar::make_lw_shared<cql::CqlSessionConfiguration>(),
		seastar::make_lw_shared<cql::CqlNodeConfiguration>(
			cql::CqlNodeConfiguration()
				.setAddress(DB_SSL_1_IP, DB_SSL_1_PORT)
				.setUseSsl(true)));
	return connection->ready().then_wrapped([] (auto&& f) {
		ASSERT_THROWS_CONTAINS(
			cql::CqlConnectionInitializeException, f.get(),
			"server required authentication: org.apache.cassandra.auth.PasswordAuthenticator");
	});
}

TEST_FUTURE(TestConnection, waitForReadyPasswordAuthenticateFailed) {
	auto connection = seastar::make_lw_shared<cql::CqlConnection>(
		seastar::make_lw_shared<cql::CqlSessionConfiguration>(),
		seastar::make_lw_shared<cql::CqlNodeConfiguration>(
			cql::CqlNodeConfiguration()
				.setAddress(DB_SSL_1_IP, DB_SSL_1_PORT)
				.setUseSsl(true)
				.setUseCompression(true)
				.setPasswordAuthentication("cassandra", "cassandra_")));
	return connection->ready().then_wrapped([] (auto&& f) {
		ASSERT_THROWS_CONTAINS(
			cql::CqlConnectionInitializeException, f.get(),
			"authenticate with password failed: CqlErrorMessage");
	});
}

TEST(TestConnection, openStream) {
	static const std::size_t testMaxStream = 5;
	auto connection = seastar::make_lw_shared<cql::CqlConnection>(
		seastar::make_lw_shared<cql::CqlSessionConfiguration>(),
		seastar::make_lw_shared<cql::CqlNodeConfiguration>(
			cql::CqlNodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
				.setMaxStreams(testMaxStream)));
	for (std::size_t i = 0; i < 3; ++i) {
		// test 3 round
		std::vector<cql::CqlConnectionStream> streams;
		for (std::size_t j = 1; j < testMaxStream; ++j) {
			// stream 0 is reserved
			auto stream = connection->openStream();
			ASSERT_TRUE(stream.isValid());
			ASSERT_GT(stream.getStreamId(), 0);
			ASSERT_LT(stream.getStreamId(), testMaxStream);
			for (const auto& existStream : streams) {
				ASSERT_NE(stream.getStreamId(), existStream.getStreamId());
			}
			streams.emplace_back(std::move(stream));
		}
		auto invalidStream = connection->openStream();
		ASSERT_FALSE(invalidStream.isValid());
	}
}

