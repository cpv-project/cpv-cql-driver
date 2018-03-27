#include <CQLDriver/Common/Command.hpp>
#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCommand, isValid) {
	cql::Command a("use a");
	cql::Command b("use b");
	ASSERT_TRUE(a.isValid());
	ASSERT_TRUE(b.isValid());
	b = std::move(a);
	ASSERT_FALSE(a.isValid());
	ASSERT_TRUE(b.isValid());
	a = std::move(b);
	ASSERT_TRUE(a.isValid());
	ASSERT_FALSE(b.isValid());
}

TEST(TestCommand, query) {
	{
		cql::Command command("use a;");
		ASSERT_EQ(command.getQuery(), "use a;");
	}
	{
		cql::Command command(std::string("use b;"));
		ASSERT_EQ(command.getQuery(), "use b;");
	}
}

TEST(TestCommand, consistencyLevel) {
	{
		cql::Command command("use a;");
		ASSERT_FALSE(command.getConsistency().has_value());
	}
	{
		auto command = cql::Command("use a;")
			.setConsistency(cql::ConsistencyLevel::One);
		ASSERT_TRUE(command.getConsistency().has_value());
		ASSERT_EQ(*command.getConsistency(), cql::ConsistencyLevel::One);
	}
}

TEST(TestCommand, pageSize) {
	{
		cql::Command command("use a;");
		ASSERT_FALSE(command.getPageSize().has_value());
	}
	{
		auto command = cql::Command("use a;")
			.setPageSize(123);
		ASSERT_TRUE(command.getPageSize().has_value());
		ASSERT_EQ(*command.getPageSize(), 123);
	}
}

TEST(TestCommand, pagingState) {
	{
		cql::Command command("use a;");
		ASSERT_EQ(command.getPagingState(), "");
	}
	{
		auto command = cql::Command("use a;")
			.setPagingState("abc");
		ASSERT_EQ(command.getPagingState(), "abc");
	}
}

TEST(TestCommand, parameters) {
	{
		cql::Command command("use a;");
		ASSERT_EQ(command.getParameterCount(), 0);
		ASSERT_EQ(command.getParameters(), "");
	}
	{
		auto command = cql::Command("insert into a (k, v) values (?, ?);")
			.addParameter(cql::Int(123))
			.addParameter(cql::Text("abc"));
		ASSERT_EQ(command.getParameterCount(), 2);
		ASSERT_EQ(command.getParameters(), makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc"));
	}
	{
		auto command = cql::Command("insert into b (k, c, v) values (?, ?, ?);")
			.addParameter(cql::Int(123))
			.addParameters(cql::Int(255), cql::Text("abc"));
		ASSERT_EQ(command.getParameterCount(), 3);
		ASSERT_EQ(command.getParameters(), makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x04\x00\x00\x00\xff"
			"\x00\x00\x00\x03""abc"));
	}
}

TEST(TestCommand, serialConsistencyLevel) {
	{
		cql::Command command("use a;");
		ASSERT_FALSE(command.getSerialConsistency().has_value());
	}
	{
		auto command = cql::Command("use a;")
			.setSerialConsistency(cql::ConsistencyLevel::Serial);
		ASSERT_TRUE(command.getSerialConsistency().has_value());
		ASSERT_EQ(*command.getSerialConsistency(), cql::ConsistencyLevel::Serial);
	}
	{
		auto command = cql::Command("use a;")
			.setSerialConsistency(cql::ConsistencyLevel::LocalSerial);
		ASSERT_TRUE(command.getSerialConsistency().has_value());
		ASSERT_EQ(*command.getSerialConsistency(), cql::ConsistencyLevel::LocalSerial);
	}
}

TEST(TestCommand, defaultTimestamp) {
	{
		cql::Command command("use a;");
		ASSERT_FALSE(command.getDefaultTimestamp().has_value());
	}
	{
		auto now = std::chrono::system_clock::now();
		auto command = cql::Command("use a;")
			.setDefaultTimestamp(now);
		ASSERT_TRUE(command.getDefaultTimestamp().has_value());
		ASSERT_EQ(*command.getDefaultTimestamp(), now);
	}
}

TEST(TestCommand, maxRetries) {
	{
		cql::Command command("use a;");
		ASSERT_EQ(command.getMaxRetries(), 0);
	}
	{
		auto command = cql::Command("use a;")
			.setMaxRetries(2);
		ASSERT_EQ(command.getMaxRetries(), 2);
	}
}

