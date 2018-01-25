#include <Common/CqlNodeCollectionImpl.hpp>
#include <LowLevel/CqlConnectionPool.hpp>
#include <LowLevel/CqlConnection.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST_FUTURE(TestCqlConnectionPool, tryGetConnection) {
	static const std::size_t testMaxStream = 5;
	auto connectionPool = seastar::make_lw_shared<cql::CqlConnectionPool>(
		seastar::make_lw_shared<cql::CqlSessionConfiguration>(cql::CqlSessionConfiguration()),
		seastar::make_shared<cql::CqlNodeCollectionImpl>(
			std::vector<cql::CqlNodeConfiguration>({
				cql::CqlNodeConfiguration()
					.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
					.setMaxStreams(testMaxStream)
			})));
	return seastar::futurize_apply([connectionPool] {
		auto result = connectionPool->tryGetConnection();
		ASSERT_TRUE(result.first.get() == nullptr);
	}).then([connectionPool] {
		return connectionPool->getConnection().discard_result();
	}).then([connectionPool] {
		for (std::size_t i = 0; i < 3; ++i) {
			std::vector<cql::CqlConnectionStream> streams;
			for (std::size_t j = 1; j < testMaxStream; ++j) {
				auto result = connectionPool->tryGetConnection();
				ASSERT_TRUE(result.first.get() != nullptr);
				streams.emplace_back(std::move(result.second));
			}
			{
				auto result = connectionPool->tryGetConnection();
				ASSERT_FALSE(result.first.get() != nullptr);
			}
		}
	});
}

TEST_FUTURE(TestCqlConnectionPool, getConnection) {
	static const std::size_t testMaxStream = 5;
	static const std::size_t testMaxInUseStreams = (testMaxStream-1)*3;
	auto connectionPool = seastar::make_lw_shared<cql::CqlConnectionPool>(
		seastar::make_lw_shared<cql::CqlSessionConfiguration>(cql::CqlSessionConfiguration()),
		seastar::make_shared<cql::CqlNodeCollectionImpl>(
			std::vector<cql::CqlNodeConfiguration>({
				cql::CqlNodeConfiguration()
					.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
					.setMaxStreams(testMaxStream),
				cql::CqlNodeConfiguration()
					.setAddress(DB_SSL_1_IP, DB_SSL_1_PORT)
					.setMaxStreams(testMaxStream)
					.setUseSsl(true)
					.setPasswordAuthentication("cassandra", "cassandra"),
				cql::CqlNodeConfiguration()
					.setAddress(DB_SSL_2_IP, DB_SSL_2_PORT)
					.setMaxStreams(testMaxStream)
					.setUseSsl(true)
					.setPasswordAuthentication("cassandra", "cassandra"),
			})));
	return seastar::do_with(
		std::move(connectionPool),
		std::vector<cql::CqlConnectionStream>(),
		static_cast<std::size_t>(0),
		[] (auto& connectionPool, auto& streams, auto& count) {
			return seastar::repeat([&connectionPool, &streams, &count] {
				if (count % testMaxInUseStreams == 0) {
					streams.clear();
				}
				return connectionPool->getConnection()
				.then([&streams] (auto&& connection, auto&& stream) {
					ASSERT_TRUE(connection.get() != nullptr);
					ASSERT_TRUE(stream.isValid());
					streams.emplace_back(std::move(stream));
				}).then([&count] {
					if (++count >= testMaxInUseStreams * 3) {
						return seastar::stop_iteration::yes;
					} else {
						return seastar::stop_iteration::no;
					}
				});
			});
		});
}

TEST(TestCqlConnectionPool, getConnectionFailed) {

}

TEST(TestCqlConnectionPool, getConnectionWithNotify) {

}

TEST(TestCqlConnectionPool, getConnectionWithTimer) {

}

