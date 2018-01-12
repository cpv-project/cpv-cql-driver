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

TEST(TestConnection, openStream) {
	std::size_t testMaxStream = 5;
	auto connection = seastar::make_shared<cql::CqlConnection>(
		seastar::make_shared<cql::CqlSessionConfiguration>(),
		seastar::make_shared<cql::CqlNodeConfiguration>(
			cql::CqlNodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
				.setMaxStream(testMaxStream)));
	for (std::size_t i = 0; i < 3; ++i) {
		// test 3 round
		std::vector<cql::CqlConnection::Stream> streams;
		for (std::size_t j = 1; j < testMaxStream; ++j) {
			// stream 0 is reserved
			auto stream = connection->openStream();
			ASSERT_TRUE(stream.isValid());
			streams.emplace_back(std::move(stream));
		}
		auto invalidStream = connection->openStream();
		ASSERT_FALSE(invalidStream.isValid());
	}
}

