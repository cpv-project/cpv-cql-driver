#include <CQLDriver/Common/Exceptions/ConnectionInitializeException.hpp>
#include <LowLevel/RequestMessages/RequestMessageFactory.hpp>
#include <LowLevel/RequestMessages/PrepareMessage.hpp>
#include <LowLevel/ResponseMessages/ResultMessage.hpp>
#include <LowLevel/Connection.hpp>
#include <TestUtility/GTestUtils.hpp>
#include <core/sleep.hh>

TEST_FUTURE(TestConnection, waitForReadySimple) {
	auto connection = seastar::make_lw_shared<cql::Connection>(
		seastar::make_lw_shared<cql::SessionConfiguration>(),
		seastar::make_lw_shared<cql::NodeConfiguration>(
			cql::NodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)),
		seastar::make_lw_shared<cql::MetricsData>());
	return connection->ready();
}

TEST_FUTURE(TestConnection, waitForReadySSL) {
	auto connection = seastar::make_lw_shared<cql::Connection>(
		seastar::make_lw_shared<cql::SessionConfiguration>(),
		seastar::make_lw_shared<cql::NodeConfiguration>(
			cql::NodeConfiguration()
				.setAddress(DB_SSL_1_IP, DB_SSL_1_PORT)
				.setUseSSL(true)
				.setUseCompression(true)
				.setPasswordAuthentication("cassandra", "cassandra")),
		seastar::make_lw_shared<cql::MetricsData>());
	return connection->ready();
}

TEST_FUTURE(TestConnection, waitForReadyConnectFailed) {
	auto connection = seastar::make_lw_shared<cql::Connection>(
		seastar::make_lw_shared<cql::SessionConfiguration>(),
		seastar::make_lw_shared<cql::NodeConfiguration>(
			cql::NodeConfiguration()
				.setAddress("host.not.exist", 123)),
		seastar::make_lw_shared<cql::MetricsData>());
	return connection->ready().then_wrapped([] (auto&& f) {
		ASSERT_THROWS_CONTAINS(
			cql::ConnectionInitializeException, f.get(),
			"connect to host.not.exist:123 failed:");
	});
}

TEST_FUTURE(TestConnection, waitForReadyAllowAllAuthenticateFailed) {
	auto connection = seastar::make_lw_shared<cql::Connection>(
		seastar::make_lw_shared<cql::SessionConfiguration>(),
		seastar::make_lw_shared<cql::NodeConfiguration>(
			cql::NodeConfiguration()
				.setAddress(DB_SSL_1_IP, DB_SSL_1_PORT)
				.setUseSSL(true)),
		seastar::make_lw_shared<cql::MetricsData>());
	return connection->ready().then_wrapped([] (auto&& f) {
		ASSERT_THROWS_CONTAINS(
			cql::ConnectionInitializeException, f.get(),
			"server required authentication: org.apache.cassandra.auth.PasswordAuthenticator");
	});
}

TEST_FUTURE(TestConnection, waitForReadyPasswordAuthenticateFailed) {
	auto connection = seastar::make_lw_shared<cql::Connection>(
		seastar::make_lw_shared<cql::SessionConfiguration>(),
		seastar::make_lw_shared<cql::NodeConfiguration>(
			cql::NodeConfiguration()
				.setAddress(DB_SSL_1_IP, DB_SSL_1_PORT)
				.setUseSSL(true)
				.setUseCompression(true)
				.setPasswordAuthentication("cassandra", "cassandra_")),
		seastar::make_lw_shared<cql::MetricsData>());
	return connection->ready().then_wrapped([] (auto&& f) {
		ASSERT_THROWS_CONTAINS(
			cql::ConnectionInitializeException, f.get(),
			"authenticate with password failed: ErrorMessage");
	});
}

TEST_FUTURE(TestConnection, waitForReadySetKeySpace) {
	auto connection = seastar::make_lw_shared<cql::Connection>(
		seastar::make_lw_shared<cql::SessionConfiguration>(
			cql::SessionConfiguration()
				.setDefaultKeySpace("system")),
		seastar::make_lw_shared<cql::NodeConfiguration>(
			cql::NodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)),
		seastar::make_lw_shared<cql::MetricsData>());
	return connection->ready();
}

TEST_FUTURE(TestConnection, waitForReadySetKeySpaceNotExists) {
	auto connection = seastar::make_lw_shared<cql::Connection>(
		seastar::make_lw_shared<cql::SessionConfiguration>(
			cql::SessionConfiguration()
				.setDefaultKeySpace("notExistskeySpaceQWERT")),
		seastar::make_lw_shared<cql::NodeConfiguration>(
			cql::NodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)),
		seastar::make_lw_shared<cql::MetricsData>());
	return connection->ready().then_wrapped([] (auto&& f) {
		ASSERT_THROWS_CONTAINS(
			cql::ConnectionInitializeException, f.get(),
			"Keyspace 'notexistskeyspaceqwert' does not exist");
	});
}

TEST(TestConnection, openStream) {
	static const std::size_t testMaxStream = 5;
	auto connection = seastar::make_lw_shared<cql::Connection>(
		seastar::make_lw_shared<cql::SessionConfiguration>(),
		seastar::make_lw_shared<cql::NodeConfiguration>(
			cql::NodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)
				.setMaxStreams(testMaxStream)),
		seastar::make_lw_shared<cql::MetricsData>());
	for (std::size_t i = 0; i < 3; ++i) {
		// test 3 round
		std::vector<cql::ConnectionStream> streams;
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

TEST_FUTURE(TestConnection, prepareQuery) {
	auto connection = seastar::make_lw_shared<cql::Connection>(
		seastar::make_lw_shared<cql::SessionConfiguration>(),
		seastar::make_lw_shared<cql::NodeConfiguration>(
			cql::NodeConfiguration()
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)),
		seastar::make_lw_shared<cql::MetricsData>());
	auto stream = connection->openStream();
	return seastar::do_with(
		std::move(connection), std::move(stream), std::string(),
		[] (auto& connection, auto& stream, auto& queryId) {
		return connection->ready().then([&connection, &stream] {
			// send PREPARE
			auto prepareMessage = (
				cql::RequestMessageFactory::makeRequestMessage<cql::PrepareMessage>());
			prepareMessage->getQuery().set("use system;");
			return connection->sendMessage(std::move(prepareMessage), stream);
		}).then([&connection, &stream] {
			// receive RESULT
			return connection->waitNextMessage(stream);
		}).then([&queryId] (auto message) {
			// handle RESULT
			ASSERT_EQ(message->getHeader().getOpCode(), cql::MessageType::Result);
			auto resultMessage = std::move(message).template cast<cql::ResultMessage>();
			ASSERT_EQ(resultMessage->getKind(), cql::ResultKind::Prepared);
			queryId = resultMessage->getPreparedQueryId().get();
			ASSERT_FALSE(queryId.empty());
		}).then([&connection, &stream] {
			// resend PREPARE
			auto prepareMessage = (
				cql::RequestMessageFactory::makeRequestMessage<cql::PrepareMessage>());
			prepareMessage->getQuery().set("use system;");
			return connection->sendMessage(std::move(prepareMessage), stream);
		}).then([&connection, &stream] {
			// receive RESULT
			return connection->waitNextMessage(stream);
		}).then([&queryId] (auto message) {
			// handle RESULT, check if the prepared query id is same
			ASSERT_EQ(message->getHeader().getOpCode(), cql::MessageType::Result);
			auto resultMessage = std::move(message).template cast<cql::ResultMessage>();
			ASSERT_EQ(resultMessage->getKind(), cql::ResultKind::Prepared);
			ASSERT_EQ(queryId, resultMessage->getPreparedQueryId().get());
		});
	});
}

TEST_FUTURE(TestConnection, compression) {
	auto connection = seastar::make_lw_shared<cql::Connection>(
		seastar::make_lw_shared<cql::SessionConfiguration>(),
		seastar::make_lw_shared<cql::NodeConfiguration>(
			cql::NodeConfiguration()
				.setUseCompression(true)
				.setAddress(DB_SIMPLE_IP, DB_SIMPLE_PORT)),
		seastar::make_lw_shared<cql::MetricsData>());
	auto stream = connection->openStream();
	return seastar::do_with(
		std::move(connection), std::move(stream),
		[] (auto& connection, auto& stream) {
		return connection->ready().then([&connection, &stream] {
			// send PREPARE
			auto prepareMessage = (
				cql::RequestMessageFactory::makeRequestMessage<cql::PrepareMessage>());
			prepareMessage->getQuery().set("use system;");
			return connection->sendMessage(std::move(prepareMessage), stream);
		}).then([&connection, &stream] {
			// receive RESULT
			return connection->waitNextMessage(stream);
		}).then([] (auto message) {
			// handle RESULT
			ASSERT_EQ(message->getHeader().getOpCode(), cql::MessageType::Result);
			auto resultMessage = std::move(message).template cast<cql::ResultMessage>();
			ASSERT_EQ(resultMessage->getKind(), cql::ResultKind::Prepared);
			ASSERT_FALSE(resultMessage->getPreparedQueryId().get().empty());
		});
	});
}

