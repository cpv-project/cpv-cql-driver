#include <CQLDriver/Common/BatchCommand.hpp>
#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestBatchCommand, isValid) {
	cql::BatchCommand a;
	cql::BatchCommand b;
	ASSERT_TRUE(a.isValid());
	ASSERT_TRUE(b.isValid());
	b = std::move(a);
	ASSERT_FALSE(a.isValid());
	ASSERT_TRUE(b.isValid());
	a = std::move(b);
	ASSERT_TRUE(a.isValid());
	ASSERT_FALSE(b.isValid());
}

TEST(TestBatchCommand, type) {
	{
		cql::BatchCommand command;
		ASSERT_EQ(command.getType(), cql::BatchType::Logged);
	}
	{
		auto command = cql::BatchCommand()
			.setType(cql::BatchType::Counter);
		ASSERT_EQ(command.getType(), cql::BatchType::Counter);
	}
}

TEST(TestBatchCommand, consistencyLevel) {
	{
		cql::BatchCommand command;
		ASSERT_EQ(command.getConsistency(), cql::ConsistencyLevel::Any);
	}
	{
		auto command = cql::BatchCommand()
			.setConsistency(cql::ConsistencyLevel::Quorum);
		ASSERT_EQ(command.getConsistency(), cql::ConsistencyLevel::Quorum);
	}
}

TEST(TestBatchCommand, query) {
	{
		cql::BatchCommand command;
		ASSERT_EQ(command.getQueryCount(), 0);
	}
	{
		auto command = cql::BatchCommand()
			.addQuery("insert into a (k, v) values (123, 'abc');")
			.addQuery("insert into b (k, c, v) values (100, 0, 'asd');");
		ASSERT_EQ(command.getQueryCount(), 2);
		ASSERT_EQ(
			std::string(command.getQuery(0).first, command.getQuery(0).second),
			"insert into a (k, v) values (123, 'abc');");
		ASSERT_EQ(
			std::string(command.getQuery(1).first, command.getQuery(1).second),
			"insert into b (k, c, v) values (100, 0, 'asd');");
	}
}

TEST(TestBatchCommand, parameters) {
	auto command = cql::BatchCommand()
		.addQuery("insert into a (k, v) values (?, ?);")
			.addParameters(cql::Int(123), cql::Text("abc"))
		.addQuery("insert into b (k, c, v) values (?, ?, ?);")
			.openParameterSet()
				.addParameter(cql::Int(100))
				.addParameter(cql::Int(0))
				.addParameter(cql::Text("asd"))
			.openParameterSet()
				.addParameter(cql::Int(127))
				.addParameter(cql::Int(255))
				.addParameter(cql::Text("qwert"));
	ASSERT_EQ(command.getQueryCount(), 2);
	ASSERT_EQ(
		std::string(command.getQuery(0).first, command.getQuery(0).second),
		"insert into a (k, v) values (?, ?);");
	ASSERT_EQ(command.getParameterSets(0).size(), 1);
	ASSERT_EQ(command.getParameterSets(0).at(0).first, 2);
	ASSERT_EQ(command.getParameterSets(0).at(0).second, makeTestString(
		"\x00\x00\x00\x04\x00\x00\x00\x7b"
		"\x00\x00\x00\x03""abc"));
	ASSERT_EQ(
		std::string(command.getQuery(1).first, command.getQuery(1).second),
		"insert into b (k, c, v) values (?, ?, ?);");
	ASSERT_EQ(command.getParameterSets(1).size(), 2);
	ASSERT_EQ(command.getParameterSets(1).at(0).first, 3);
	ASSERT_EQ(command.getParameterSets(1).at(0).second, makeTestString(
		"\x00\x00\x00\x04\x00\x00\x00\x64"
		"\x00\x00\x00\x04\x00\x00\x00\x00"
		"\x00\x00\x00\x03""asd"));
	ASSERT_EQ(command.getParameterSets(1).at(1).first, 3);
	ASSERT_EQ(command.getParameterSets(1).at(1).second, makeTestString(
		"\x00\x00\x00\x04\x00\x00\x00\x7f"
		"\x00\x00\x00\x04\x00\x00\x00\xff"
		"\x00\x00\x00\x05""qwert"));
}

TEST(TestBatchCommand, parametersError) {
	{
		cql::BatchCommand command;
		ASSERT_THROWS_CONTAINS(
			cql::LogicException,
			command.addParameter(cql::Int(1)),
			"please call addQuery before addParameters");
	}
	{
		cql::BatchCommand command;
		ASSERT_THROWS(std::out_of_range, command.getQuery(0));
		ASSERT_THROWS(std::out_of_range, command.getParameterSets(0));
	}
}

TEST(TestBatchCommand, serialConsistencyLevel) {
	{
		cql::BatchCommand command;
		ASSERT_EQ(command.getSerialConsistency().first, cql::ConsistencyLevel::Serial);
		ASSERT_EQ(command.getSerialConsistency().second, false);
	}
	{
		auto command = cql::BatchCommand()
			.setSerialConsistency(cql::ConsistencyLevel::Serial);
		ASSERT_EQ(command.getSerialConsistency().first, cql::ConsistencyLevel::Serial);
		ASSERT_EQ(command.getSerialConsistency().second, true);
	}
	{
		auto command = cql::BatchCommand()
			.setSerialConsistency(cql::ConsistencyLevel::LocalSerial);
		ASSERT_EQ(command.getSerialConsistency().first, cql::ConsistencyLevel::LocalSerial);
		ASSERT_EQ(command.getSerialConsistency().second, true);
	}
}

TEST(TestBatchCommand, defaultTimestamp) {
	{
		cql::BatchCommand command;
		ASSERT_EQ(command.getDefaultTimestamp().second, false);
	}
	{
		auto now = std::chrono::system_clock::now();
		auto command = cql::BatchCommand()
			.setDefaultTimestamp(now);
		ASSERT_EQ(command.getDefaultTimestamp().first, now);
		ASSERT_EQ(command.getDefaultTimestamp().second, true);
	}
}

TEST(TestBatchCommand, maxRetries) {
	{
		cql::BatchCommand command;
		ASSERT_EQ(command.getMaxRetries(), 0);
	}
	{
		auto command = cql::BatchCommand()
			.setMaxRetries(2);
		ASSERT_EQ(command.getMaxRetries(), 2);
	}
}

