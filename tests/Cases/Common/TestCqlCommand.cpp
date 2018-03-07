#include <CqlDriver/Common/CqlCommand.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlInt.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlText.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlCommand, isValid) {
	cql::CqlCommand a("use a");
	cql::CqlCommand b("use b");
	ASSERT_TRUE(a.isValid());
	ASSERT_TRUE(b.isValid());
	b = std::move(a);
	ASSERT_FALSE(a.isValid());
	ASSERT_TRUE(b.isValid());
	a = std::move(b);
	ASSERT_TRUE(a.isValid());
	ASSERT_FALSE(b.isValid());
}

TEST(TestCqlCommand, query) {
	{
		cql::CqlCommand command("use a;");
		ASSERT_EQ(
			seastar::sstring(command.getQuery().first, command.getQuery().second),
			"use a;");
	}
	{
		cql::CqlCommand command(seastar::sstring("use b;"));
		ASSERT_EQ(
			seastar::sstring(command.getQuery().first, command.getQuery().second),
			"use b;");
	}
}

TEST(TestCqlCommand, consistencyLevel) {
	{
		cql::CqlCommand command("use a;");
		ASSERT_EQ(command.getConsistencyLevel(), cql::CqlConsistencyLevel::Any);
	}
	{
		auto command = cql::CqlCommand("use a;")
			.setConsistencyLevel(cql::CqlConsistencyLevel::Quorum);
		ASSERT_EQ(command.getConsistencyLevel(), cql::CqlConsistencyLevel::Quorum);
	}
}

TEST(TestCqlCommand, pageSize) {
	{
		cql::CqlCommand command("use a;");
		ASSERT_EQ(command.getPageSize().first, 0);
		ASSERT_EQ(command.getPageSize().second, false);
	}
	{
		auto command = cql::CqlCommand("use a;")
			.setPageSize(123);
		ASSERT_EQ(command.getPageSize().first, 123);
		ASSERT_EQ(command.getPageSize().second, true);
	}
}

TEST(TestCqlCommand, pageState) {
	{
		cql::CqlCommand command("use a;");
		ASSERT_EQ(command.getPageState(), "");
	}
	{
		auto command = cql::CqlCommand("use a;")
			.setPageState("abc");
		ASSERT_EQ(command.getPageState(), "abc");
	}
}

TEST(TestCqlCommand, parameters) {
	{
		cql::CqlCommand command("use a;");
		ASSERT_EQ(command.getParameterCount(), 0);
		ASSERT_EQ(command.getParameters(), "");
	}
	{
		auto command = cql::CqlCommand("insert into a (k, v) values (?, ?);")
			.addParameter(cql::CqlInt(123))
			.addParameter(cql::CqlText("abc"));
		ASSERT_EQ(command.getParameterCount(), 2);
		ASSERT_EQ(command.getParameters(), makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc"));
	}
	{
		auto command = cql::CqlCommand("insert into b (k, c, v) values (?, ?, ?);")
			.addParameter(cql::CqlInt(123))
			.addParameters(cql::CqlInt(255), cql::CqlText("abc"));
		ASSERT_EQ(command.getParameterCount(), 3);
		ASSERT_EQ(command.getParameters(), makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x04\x00\x00\x00\xff"
			"\x00\x00\x00\x03""abc"));
	}
}

