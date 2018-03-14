#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <LowLevel/ProtocolTypes/ProtocolQueryParameters.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolQueryParameters, getset) {
	cql::ProtocolQueryParameters value;
	value.setCommand(cql::Command("use a")
		.setConsistency(cql::ConsistencyLevel::One)
		.addParameters(cql::Int(123))
		.setPageSize(100)
		.setPagingState("state"));
	auto& command = value.getCommand();
	ASSERT_EQ(command.getConsistency(), cql::ConsistencyLevel::One);
	ASSERT_EQ(command.getParameterCount(), 1);
	ASSERT_EQ(command.getParameters(), makeTestString("\x00\x00\x00\x04\x00\x00\x00\x7b"));
	ASSERT_EQ(command.getPageSize().first, 100);
	ASSERT_TRUE(command.getPageSize().second);
	ASSERT_EQ(command.getPagingState(), "state");
}

TEST(TestProtocolQueryParameters, move) {
	cql::ProtocolQueryParameters a;
	a.setCommand(cql::Command("use a;"));
	ASSERT_TRUE(a.getCommand().isValid());

	cql::ProtocolQueryParameters b = std::move(a);
	ASSERT_FALSE(a.getCommand().isValid());
	ASSERT_TRUE(b.getCommand().isValid());

	cql::Command command("use b;");
	b.setCommandRef(command);
	ASSERT_EQ(&command, &b.getCommand());
	ASSERT_TRUE(command.isValid());
	ASSERT_TRUE(b.getCommand().isValid());

	a = std::move(b);
	ASSERT_EQ(&command, &a.getCommand());
	ASSERT_TRUE(command.isValid());
	ASSERT_TRUE(a.getCommand().isValid());
	ASSERT_FALSE(b.getCommand().isValid());
}

TEST(TestProtocolQueryParameters, encode) {
	{
		cql::ProtocolQueryParameters value;
		value.setSkipMetadata(true);
		value.setCommand(cql::Command("use a")
			.setConsistency(cql::ConsistencyLevel::One)
			.addParameters(cql::Text("a"), cql::Text("b"))
			.setPageSize(100)
			.setPagingState("state"));
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x01"
			"\x0f"
			"\x00\x02"
			"\x00\x00\x00\x01""a"
			"\x00\x00\x00\x01""b"
			"\x00\x00\x00\x64"
			"\x00\x00\x00\x05""state"));
	}
	{
		cql::ProtocolQueryParameters value;
		value.setSkipMetadata(false);
		value.setCommand(cql::Command("use a")
			.setConsistency(cql::ConsistencyLevel::Two)
			.addParameters(cql::Text("a"))
			.setSerialConsistency(cql::ConsistencyLevel::LocalSerial)
			.setDefaultTimestamp(
				std::chrono::system_clock::from_time_t(0) +
				std::chrono::microseconds(123)));
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x02"
			"\x31"
			"\x00\x01"
			"\x00\x00\x00\x01""a"
			"\x00\x09"
			"\x00\x00\x00\x00\x00\x00\x00\x7b"));
	}
}

TEST(TestProtocolQueryParameters, decode) {
	cql::ProtocolQueryParameters value;
	{
		auto data = makeTestString(
			"\x00\x01"
			"\x0f"
			"\x00\x02"
			"\x00\x00\x00\x01""a"
			"\x00\x00\x00\x01""b"
			"\x00\x00\x00\x64"
			"\x00\x00\x00\x05""state");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		auto& command = value.getCommand();
		ASSERT_TRUE(command.isValid());
		ASSERT_EQ(command.getConsistency(), cql::ConsistencyLevel::One);
		ASSERT_EQ(value.getFlags(),
			cql::QueryParametersFlags::SkipMetadata |
			cql::QueryParametersFlags::WithValues |
			cql::QueryParametersFlags::WithPageSize |
			cql::QueryParametersFlags::WithPagingState);
		ASSERT_EQ(command.getParameterCount(), 2);
		ASSERT_EQ(command.getParameters(), makeTestString(
			"\x00\x00\x00\x01""a"
			"\x00\x00\x00\x01""b"));
		ASSERT_EQ(command.getPageSize().first, 100);
		ASSERT_TRUE(command.getPageSize().second);
		ASSERT_EQ(command.getPagingState(), "state");
		ASSERT_FALSE(command.getSerialConsistency().second);
		ASSERT_FALSE(command.getDefaultTimestamp().second);
	}
	{
		auto data = makeTestString(
			"\x00\x02"
			"\x31"
			"\x00\x01"
			"\x00\x00\x00\x01""a"
			"\x00\x09"
			"\x00\x00\x00\x00\x00\x00\x00\x7b");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		auto& command = value.getCommand();
		ASSERT_TRUE(command.isValid());
		ASSERT_EQ(command.getConsistency(), cql::ConsistencyLevel::Two);
		ASSERT_EQ(value.getFlags(),
			cql::QueryParametersFlags::WithValues |
			cql::QueryParametersFlags::WithSerialConsistency |
			cql::QueryParametersFlags::WithDefaultTimestamp);
		ASSERT_EQ(command.getParameterCount(), 1);
		ASSERT_EQ(command.getParameters(), makeTestString("\x00\x00\x00\x01""a"));
		ASSERT_FALSE(command.getPageSize().second);
		ASSERT_EQ(command.getPagingState(), "");
		ASSERT_EQ(command.getSerialConsistency().first,
			cql::ConsistencyLevel::LocalSerial);
		ASSERT_TRUE(command.getSerialConsistency().second);
		ASSERT_EQ(command.getDefaultTimestamp().first,
			std::chrono::system_clock::from_time_t(0) + std::chrono::microseconds(123));
		ASSERT_TRUE(command.getDefaultTimestamp().second);
	}
}

