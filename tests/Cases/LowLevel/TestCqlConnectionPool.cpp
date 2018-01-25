#include <core/sleep.hh>
#include <CqlDriver/Common/Exceptions/CqlConnectionNotAvailableException.hpp>
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
	static const std::size_t testMinPoolSize = 1;
	static const std::size_t testMaxPoolSize = 3;
	static const std::size_t totalMaxStreams = (testMaxStream-1)*testMaxPoolSize;
	auto connectionPool = seastar::make_lw_shared<cql::CqlConnectionPool>(
		seastar::make_lw_shared<cql::CqlSessionConfiguration>(
			cql::CqlSessionConfiguration()
				.setMinPoolSize(testMinPoolSize)
				.setMaxPoolSize(testMaxPoolSize)
				.setMaxWaitersAfterConnectionsExhausted(0)),
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

TEST_FUTURE(TestCqlConnectionPool, getConnectionFailed) {
	static const std::size_t testMaxStream = 5;
	static const std::size_t testMinPoolSize = 1;
	static const std::size_t testMaxPoolSize = 3;
	static const std::size_t totalMaxStreams = (testMaxStream-1)*testMaxPoolSize;
	auto connectionPool = seastar::make_lw_shared<cql::CqlConnectionPool>(
		seastar::make_lw_shared<cql::CqlSessionConfiguration>(
			cql::CqlSessionConfiguration()
				.setMinPoolSize(testMinPoolSize)
				.setMaxPoolSize(testMaxPoolSize)
				.setMaxWaitersAfterConnectionsExhausted(0)),
		seastar::make_shared<cql::CqlNodeCollectionImpl>(
			std::vector<cql::CqlNodeConfiguration>({
				cql::CqlNodeConfiguration()
					.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
					.setMaxStreams(testMaxStream)
			})));
	return seastar::do_with(
		std::move(connectionPool),
		std::vector<cql::CqlConnectionStream>(),
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
						ASSERT_THROWS(cql::CqlConnectionNotAvailableException, f.get());
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

TEST_FUTURE(TestCqlConnectionPool, getConnectionWithNotify) {
	static const std::size_t testMaxStream = 5;
	static const std::size_t testMinPoolSize = 1;
	static const std::size_t testMaxPoolSize = 3;
	static const std::size_t totalMaxStreams = (testMaxStream-1)*testMaxPoolSize;
	auto connectionPool = seastar::make_lw_shared<cql::CqlConnectionPool>(
		seastar::make_lw_shared<cql::CqlSessionConfiguration>(
			cql::CqlSessionConfiguration()
				.setMinPoolSize(testMinPoolSize)
				.setMaxPoolSize(testMaxPoolSize)
				.setMaxWaitersAfterConnectionsExhausted(1)),
		seastar::make_shared<cql::CqlNodeCollectionImpl>(
			std::vector<cql::CqlNodeConfiguration>({
				cql::CqlNodeConfiguration()
					.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
					.setMaxStreams(testMaxStream)
			})));
	return seastar::do_with(
		std::move(connectionPool),
		std::vector<cql::CqlConnectionStream>(),
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
							stream = {};
							connectionPool->notifyConnectionBecomeIdle(std::move(connection));
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

TEST_FUTURE(TestCqlConnectionPool, getConnectionWithTimer) {
	static const std::size_t testMaxStream = 5;
	static const std::size_t testMinPoolSize = 1;
	static const std::size_t testMaxPoolSize = 3;
	static const std::size_t totalMaxStreams = (testMaxStream-1)*testMaxPoolSize;
	auto connectionPool = seastar::make_lw_shared<cql::CqlConnectionPool>(
		seastar::make_lw_shared<cql::CqlSessionConfiguration>(
			cql::CqlSessionConfiguration()
				.setMinPoolSize(testMinPoolSize)
				.setMaxPoolSize(testMaxPoolSize)
				.setMaxWaitersAfterConnectionsExhausted(1)),
		seastar::make_shared<cql::CqlNodeCollectionImpl>(
			std::vector<cql::CqlNodeConfiguration>({
				cql::CqlNodeConfiguration()
					.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
					.setMaxStreams(testMaxStream)
			})));
	return seastar::do_with(
		std::move(connectionPool),
		std::vector<cql::CqlConnectionStream>(),
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

