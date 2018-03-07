#include <CqlDriver/Common/CqlBatchCommand.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlInt.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlText.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlBatchCommand, isValid) {
	cql::CqlBatchCommand a;
	cql::CqlBatchCommand b;
	ASSERT_TRUE(a.isValid());
	ASSERT_TRUE(b.isValid());
	b = std::move(a);
	ASSERT_FALSE(a.isValid());
	ASSERT_TRUE(b.isValid());
	a = std::move(b);
	ASSERT_TRUE(a.isValid());
	ASSERT_FALSE(b.isValid());
}

TEST(TestCqlBatchCommand, consistencyLevel) {
	{
		cql::CqlBatchCommand command;
		ASSERT_EQ(command.getConsistencyLevel(), cql::CqlConsistencyLevel::Any);
	}
	{
		auto command = cql::CqlBatchCommand()
			.setConsistencyLevel(cql::CqlConsistencyLevel::Quorum);
		ASSERT_EQ(command.getConsistencyLevel(), cql::CqlConsistencyLevel::Quorum);
	}
}

TEST(TestCqlBatchCommand, query) {
	{
		cql::CqlBatchCommand command;
		ASSERT_EQ(command.getQueryCount(), 0);
	}
	{
		auto command = cql::CqlBatchCommand()
			.addQuery("insert into a (k, v) values (123, 'abc');")
			.addQuery("insert into b (k, c, v) values (100, 0, 'asd');");
		ASSERT_EQ(command.getQueryCount(), 2);
		ASSERT_EQ(
			seastar::sstring(command.getQuery(0).first, command.getQuery(0).second),
			"insert into a (k, v) values (123, 'abc');");
		ASSERT_EQ(
			seastar::sstring(command.getQuery(1).first, command.getQuery(1).second),
			"insert into b (k, c, v) values (100, 0, 'asd');");
	}
}

TEST(TestCqlBatchCommand, parameters) {
	auto command = cql::CqlBatchCommand()
		.addQuery("insert into a (k, v) values (?, ?);")
			.addParameters(cql::CqlInt(123), cql::CqlText("abc"))
		.addQuery("insert into b (k, c, v) values (?, ?, ?);")
			.openParameterSet()
				.addParameter(cql::CqlInt(100))
				.addParameter(cql::CqlInt(0))
				.addParameter(cql::CqlText("asd"))
			.openParameterSet()
				.addParameter(cql::CqlInt(127))
				.addParameter(cql::CqlInt(255))
				.addParameter(cql::CqlText("qwert"));
	ASSERT_EQ(command.getQueryCount(), 2);
	ASSERT_EQ(
		seastar::sstring(command.getQuery(0).first, command.getQuery(0).second),
		"insert into a (k, v) values (?, ?);");
	ASSERT_EQ(command.getParameterSets(0).size(), 1);
	ASSERT_EQ(command.getParameterSets(0).at(0).first, 2);
	ASSERT_EQ(command.getParameterSets(0).at(0).second, makeTestString(
		"\x00\x00\x00\x04\x00\x00\x00\x7b"
		"\x00\x00\x00\x03""abc"));
	ASSERT_EQ(
		seastar::sstring(command.getQuery(1).first, command.getQuery(1).second),
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

TEST(TestCqlBatchCommand, parametersError) {
	{
		cql::CqlBatchCommand command;
		ASSERT_THROWS_CONTAINS(
			cql::CqlLogicException,
			command.addParameter(cql::CqlInt(1)),
			"please call addQuery before addParameters");
	}
	{
		cql::CqlBatchCommand command;
		ASSERT_THROWS(std::out_of_range, command.getQuery(0));
		ASSERT_THROWS(std::out_of_range, command.getParameterSets(0));
	}
}

TEST(TestCqlBatchCommand, serialConsistencyLevel) {
	{
		cql::CqlBatchCommand command;
		ASSERT_EQ(command.getSerialConsistencyLevel().first, cql::CqlConsistencyLevel::Serial);
		ASSERT_EQ(command.getSerialConsistencyLevel().second, false);
	}
	{
		auto command = cql::CqlBatchCommand()
			.setSerialConsistencyLevel(cql::CqlConsistencyLevel::Serial);
		ASSERT_EQ(command.getSerialConsistencyLevel().first, cql::CqlConsistencyLevel::Serial);
		ASSERT_EQ(command.getSerialConsistencyLevel().second, true);
	}
	{
		auto command = cql::CqlBatchCommand()
			.setSerialConsistencyLevel(cql::CqlConsistencyLevel::LocalSerial);
		ASSERT_EQ(command.getSerialConsistencyLevel().first, cql::CqlConsistencyLevel::LocalSerial);
		ASSERT_EQ(command.getSerialConsistencyLevel().second, true);
	}
}

TEST(TestCqlBatchCommand, defaultTimeStamp) {
	{
		cql::CqlBatchCommand command;
		ASSERT_EQ(command.getDefaultTimeStamp().second, false);
	}
	{
		auto now = std::chrono::system_clock::now();
		auto command = cql::CqlBatchCommand()
			.setDefaultTimeStamp(now);
		ASSERT_EQ(command.getDefaultTimeStamp().first, now);
		ASSERT_EQ(command.getDefaultTimeStamp().second, true);
	}
}

