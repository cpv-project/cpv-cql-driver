#include <CQLDriver/Common/BatchCommand.hpp>
#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <Common/BatchQueryData.hpp>
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
		ASSERT_FALSE(command.getConsistency().has_value());
	}
	{
		auto command = cql::BatchCommand()
			.setConsistency(cql::ConsistencyLevel::One);
		ASSERT_TRUE(command.getConsistency().has_value());
		ASSERT_EQ(*command.getConsistency(), cql::ConsistencyLevel::One);
	}
}

TEST(TestBatchCommand, query) {
	{
		cql::BatchCommand command;
		ASSERT_EQ(command.getQueries().size(), 0);
	}
	{
		auto command = cql::BatchCommand()
			.addQuery("insert into a (k, v) values (123, 'abc');")
			.addQuery("insert into b (k, c, v) values (100, 0, 'asd');");
		auto queries = command.getQueries();
		ASSERT_EQ(queries.size(), 2);
		ASSERT_EQ(queries.at(0).queryStr.get(),
			"insert into a (k, v) values (123, 'abc');");
		ASSERT_EQ(queries.at(1).queryStr.get(),
			"insert into b (k, c, v) values (100, 0, 'asd');");
	}
}

TEST(TestBatchCommand, prepare) {
	auto command = cql::BatchCommand()
		.addQuery("insert into a (k, v) values (123, 'abc');")
		.addQuery("insert into b (k, c, v) values (100, 0, 'asd');")
			.prepareQuery();
	auto queries = command.getQueries();
	ASSERT_EQ(queries.size(), 2);
	ASSERT_FALSE(queries.at(0).needPrepare.has_value());
	ASSERT_TRUE(queries.at(1).needPrepare.has_value());
	ASSERT_TRUE(*queries.at(1).needPrepare);
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
	auto queries = command.getQueries();
	ASSERT_EQ(queries.size(), 2);

	auto query_0 = queries.at(0);
	ASSERT_EQ(query_0.queryStr.get(),
		"insert into a (k, v) values (?, ?);");
	ASSERT_EQ(query_0.parameterSets.size(), 1);
	ASSERT_EQ(query_0.parameterSets.at(0).first, 2);
	ASSERT_EQ(query_0.parameterSets.at(0).second, makeTestString(
		"\x00\x00\x00\x04\x00\x00\x00\x7b"
		"\x00\x00\x00\x03""abc"));

	auto query_1 = queries.at(1);
	ASSERT_EQ(query_1.queryStr.get(),
		"insert into b (k, c, v) values (?, ?, ?);");
	ASSERT_EQ(query_1.parameterSets.size(), 2);
	ASSERT_EQ(query_1.parameterSets.at(0).first, 3);
	ASSERT_EQ(query_1.parameterSets.at(0).second, makeTestString(
		"\x00\x00\x00\x04\x00\x00\x00\x64"
		"\x00\x00\x00\x04\x00\x00\x00\x00"
		"\x00\x00\x00\x03""asd"));
	ASSERT_EQ(query_1.parameterSets.at(1).first, 3);
	ASSERT_EQ(query_1.parameterSets.at(1).second, makeTestString(
		"\x00\x00\x00\x04\x00\x00\x00\x7f"
		"\x00\x00\x00\x04\x00\x00\x00\xff"
		"\x00\x00\x00\x05""qwert"));
}

TEST(TestBatchCommand, parametersError) {
	cql::BatchCommand command;
	ASSERT_THROWS_CONTAINS(
		cql::LogicException,
		command.addParameter(cql::Int(1)),
		"please call addQuery before addParameters");
}

TEST(TestBatchCommand, serialConsistencyLevel) {
	{
		cql::BatchCommand command;
		ASSERT_FALSE(command.getSerialConsistency().has_value());
	}
	{
		auto command = cql::BatchCommand()
			.setSerialConsistency(cql::ConsistencyLevel::Serial);
		ASSERT_TRUE(command.getSerialConsistency().has_value());
		ASSERT_EQ(*command.getSerialConsistency(), cql::ConsistencyLevel::Serial);
	}
	{
		auto command = cql::BatchCommand()
			.setSerialConsistency(cql::ConsistencyLevel::LocalSerial);
		ASSERT_TRUE(command.getSerialConsistency().has_value());
		ASSERT_EQ(*command.getSerialConsistency(), cql::ConsistencyLevel::LocalSerial);
	}
}

TEST(TestBatchCommand, defaultTimestamp) {
	{
		cql::BatchCommand command;
		ASSERT_FALSE(command.getDefaultTimestamp().has_value());
	}
	{
		auto now = std::chrono::system_clock::now();
		auto command = cql::BatchCommand()
			.setDefaultTimestamp(now);
		ASSERT_TRUE(command.getDefaultTimestamp().has_value());
		ASSERT_EQ(*command.getDefaultTimestamp(), now);
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

