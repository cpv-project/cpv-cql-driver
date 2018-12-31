#include <seastar/core/sleep.hh>
#include <CQLDriver/Common/Exceptions/ConnectionNotAvailableException.hpp>
#include <Common/NodeCollectionImpl.hpp>
#include <LowLevel/ConnectionPool.hpp>
#include <LowLevel/Connection.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST_FUTURE(TestConnectionPool, tryGetConnection) {
	static const std::size_t testMaxStream = 5;
	auto connectionPool = seastar::make_lw_shared<cql::ConnectionPool>(
		seastar::make_lw_shared<cql::SessionConfiguration>(cql::SessionConfiguration()),
		seastar::make_shared<cql::NodeCollectionImpl>(
			std::vector<cql::NodeConfiguration>({
				cql::NodeConfiguration()
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
			std::vector<cql::ConnectionStream> streams;
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

TEST_FUTURE(TestConnectionPool, getConnection) {
	static const std::size_t testMaxStream = 5;
	static const std::size_t testMinPoolSize = 1;
	static const std::size_t testMaxPoolSize = 3;
	static const std::size_t totalMaxStreams = (testMaxStream-1)*testMaxPoolSize;
	auto connectionPool = seastar::make_lw_shared<cql::ConnectionPool>(
		seastar::make_lw_shared<cql::SessionConfiguration>(
			cql::SessionConfiguration()
				.setMinPoolSize(testMinPoolSize)
				.setMaxPoolSize(testMaxPoolSize)
				.setMaxWaitersAfterConnectionsExhausted(0)),
		seastar::make_shared<cql::NodeCollectionImpl>(
			std::vector<cql::NodeConfiguration>({
				cql::NodeConfiguration()
					.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
					.setMaxStreams(testMaxStream),
				cql::NodeConfiguration()
					.setAddress(DB_SSL_1_IP, DB_SSL_1_PORT)
					.setMaxStreams(testMaxStream)
					.setUseSSL(true)
					.setPasswordAuthentication("cassandra", "cassandra"),
				cql::NodeConfiguration()
					.setAddress(DB_SSL_2_IP, DB_SSL_2_PORT)
					.setMaxStreams(testMaxStream)
					.setUseSSL(true)
					.setPasswordAuthentication("cassandra", "cassandra"),
			})));
	return seastar::do_with(
		std::move(connectionPool),
		std::vector<cql::ConnectionStream>(),
		static_cast<std::size_t>(0),
		[] (auto& connectionPool, auto& streams, auto& count) {
			return seastar::repeat([&connectionPool, &streams, &count] {
				if (count % totalMaxStreams == 0) {
					streams.clear();
				}
				return connectionPool->getConnection()
				.then([&streams] (auto&& connection, auto&& stream) {
					ASSERT_TRUE(connection.get() != nullptr);
					ASSERT_TRUE(stream.isValid());
					streams.emplace_back(std::move(stream));
				}).then([&count] {
					if (++count >= totalMaxStreams * 3) {
						return seastar::stop_iteration::yes;
					} else {
						return seastar::stop_iteration::no;
					}
				});
			});
		});
}

TEST_FUTURE(TestConnectionPool, getConnectionFailed) {
	static const std::size_t testMaxStream = 5;
	static const std::size_t testMinPoolSize = 1;
	static const std::size_t testMaxPoolSize = 3;
	static const std::size_t totalMaxStreams = (testMaxStream-1)*testMaxPoolSize;
	auto connectionPool = seastar::make_lw_shared<cql::ConnectionPool>(
		seastar::make_lw_shared<cql::SessionConfiguration>(
			cql::SessionConfiguration()
				.setMinPoolSize(testMinPoolSize)
				.setMaxPoolSize(testMaxPoolSize)
				.setMaxWaitersAfterConnectionsExhausted(0)),
		seastar::make_shared<cql::NodeCollectionImpl>(
			std::vector<cql::NodeConfiguration>({
				cql::NodeConfiguration()
					.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
					.setMaxStreams(testMaxStream)
			})));
	return seastar::do_with(
		std::move(connectionPool),
		std::vector<cql::ConnectionStream>(),
		static_cast<std::size_t>(0),
		[] (auto& connectionPool, auto& streams, auto& count) {
			return seastar::repeat([&connectionPool, &streams, &count] {
				return connectionPool->getConnection()
				.then([&streams] (auto&& connection, auto&& stream) {
					ASSERT_TRUE(connection.get() != nullptr);
					ASSERT_TRUE(stream.isValid());
					streams.emplace_back(std::move(stream));
				}).then_wrapped([&count] (auto&& f) {
					if (count < totalMaxStreams) {
						f.get();
					} else {
						ASSERT_THROWS(cql::ConnectionNotAvailableException, f.get());
					}
				}).then([&count] {
					if (++count >= totalMaxStreams+3) {
						return seastar::stop_iteration::yes;
					} else {
						return seastar::stop_iteration::no;
					}
				});
			});
		});
}

TEST_FUTURE(TestConnectionPool, getConnectionThenReturn) {
	static const std::size_t testMaxStream = 5;
	static const std::size_t testMinPoolSize = 1;
	static const std::size_t testMaxPoolSize = 3;
	static const std::size_t totalMaxStreams = (testMaxStream-1)*testMaxPoolSize;
	auto connectionPool = seastar::make_lw_shared<cql::ConnectionPool>(
		seastar::make_lw_shared<cql::SessionConfiguration>(
			cql::SessionConfiguration()
				.setMinPoolSize(testMinPoolSize)
				.setMaxPoolSize(testMaxPoolSize)
				.setMaxWaitersAfterConnectionsExhausted(1)),
		seastar::make_shared<cql::NodeCollectionImpl>(
			std::vector<cql::NodeConfiguration>({
				cql::NodeConfiguration()
					.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
					.setMaxStreams(testMaxStream)
			})));
	return seastar::do_with(
		std::move(connectionPool),
		std::vector<cql::ConnectionStream>(),
		static_cast<std::size_t>(0),
		[] (auto& connectionPool, auto& streams, auto& count) {
			return seastar::repeat([&connectionPool, &streams, &count] {
				return connectionPool->getConnection()
				.then([&connectionPool, &streams, &count] (auto&& connection, auto&& stream) {
					ASSERT_TRUE(connection.get() != nullptr);
					ASSERT_TRUE(stream.isValid());
					if (count < totalMaxStreams-1) {
						streams.emplace_back(std::move(stream));
					} else {
						seastar::sleep(std::chrono::milliseconds(1))
						.then([connectionPool,
							connection=std::move(connection),
							stream=std::move(stream)] () mutable {
							connectionPool->returnConnection(std::move(connection), std::move(stream));
						});
					}
				}).then([&count] {
					if (++count >= totalMaxStreams+3) {
						return seastar::stop_iteration::yes;
					} else {
						return seastar::stop_iteration::no;
					}
				});
			});
		});
}

TEST_FUTURE(TestConnectionPool, getConnectionWithTimer) {
	static const std::size_t testMaxStream = 5;
	static const std::size_t testMinPoolSize = 1;
	static const std::size_t testMaxPoolSize = 3;
	static const std::size_t totalMaxStreams = (testMaxStream-1)*testMaxPoolSize;
	auto connectionPool = seastar::make_lw_shared<cql::ConnectionPool>(
		seastar::make_lw_shared<cql::SessionConfiguration>(
			cql::SessionConfiguration()
				.setMinPoolSize(testMinPoolSize)
				.setMaxPoolSize(testMaxPoolSize)
				.setMaxWaitersAfterConnectionsExhausted(1)),
		seastar::make_shared<cql::NodeCollectionImpl>(
			std::vector<cql::NodeConfiguration>({
				cql::NodeConfiguration()
					.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
					.setMaxStreams(testMaxStream)
			})));
	return seastar::do_with(
		std::move(connectionPool),
		std::vector<cql::ConnectionStream>(),
		static_cast<std::size_t>(0),
		[] (auto& connectionPool, auto& streams, auto& count) {
			return seastar::repeat([&connectionPool, &streams, &count] {
				return connectionPool->getConnection()
				.then([&connectionPool, &streams, &count] (auto&& connection, auto&& stream) {
					ASSERT_TRUE(connection.get() != nullptr);
					ASSERT_TRUE(stream.isValid());
					if (count < totalMaxStreams-1) {
						streams.emplace_back(std::move(stream));
					} else {
						seastar::sleep(std::chrono::milliseconds(1))
						.then([connectionPool, stream=std::move(stream)] () mutable {
							stream = {};
							// did not call returnConnection
						});
					}
				}).then([&count] {
					if (++count >= totalMaxStreams+3) {
						return seastar::stop_iteration::yes;
					} else {
						return seastar::stop_iteration::no;
					}
				});
			});
		});
}

