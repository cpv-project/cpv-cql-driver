#include <CqlDriver/Common/ColumnTypes/CqlInt.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlText.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolQueryParameters.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolQueryParameters, getset) {
	cql::CqlProtocolQueryParameters value;
	value.setCommand(cql::CqlCommand("use a")
		.setConsistencyLevel(cql::CqlConsistencyLevel::One)
		.addParameters(cql::CqlInt(123))
		.setPageSize(100)
		.setPagingState("state"));
	auto& command = value.getCommand();
	ASSERT_EQ(command.getConsistencyLevel(), cql::CqlConsistencyLevel::One);
	ASSERT_EQ(command.getParameterCount(), 1);
	ASSERT_EQ(command.getParameters(), makeTestString("\x00\x00\x00\x04\x00\x00\x00\x7b"));
	ASSERT_EQ(command.getPageSize().first, 100);
	ASSERT_TRUE(command.getPageSize().second);
	ASSERT_EQ(command.getPagingState(), "state");
}

TEST(TestCqlProtocolQueryParameters, encode) {
	{
		cql::CqlProtocolQueryParameters value;
		value.setSkipMetadata(true);
		value.setCommand(cql::CqlCommand("use a")
			.setConsistencyLevel(cql::CqlConsistencyLevel::One)
			.addParameters(cql::CqlText("a"), cql::CqlText("b"))
			.setPageSize(100)
			.setPagingState("state"));
		seastar::sstring data;
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
		cql::CqlProtocolQueryParameters value;
		value.setSkipMetadata(false);
		value.setCommand(cql::CqlCommand("use a")
			.setConsistencyLevel(cql::CqlConsistencyLevel::Two)
			.addParameters(cql::CqlText("a"))
			.setSerialConsistencyLevel(cql::CqlConsistencyLevel::LocalSerial)
			.setDefaultTimeStamp(
				std::chrono::system_clock::from_time_t(0) +
				std::chrono::microseconds(123)));
		seastar::sstring data;
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

TEST(TestCqlProtocolQueryParameters, decode) {
	cql::CqlProtocolQueryParameters value;
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
		ASSERT_EQ(command.getConsistencyLevel(), cql::CqlConsistencyLevel::One);
		ASSERT_EQ(value.getFlags(),
			cql::CqlQueryParametersFlags::SkipMetadata |
			cql::CqlQueryParametersFlags::WithValues |
			cql::CqlQueryParametersFlags::WithPageSize |
			cql::CqlQueryParametersFlags::WithPagingState);
		ASSERT_EQ(command.getParameterCount(), 2);
		ASSERT_EQ(command.getParameters(), makeTestString(
			"\x00\x00\x00\x01""a"
			"\x00\x00\x00\x01""b"));
		ASSERT_EQ(command.getPageSize().first, 100);
		ASSERT_TRUE(command.getPageSize().second);
		ASSERT_EQ(command.getPagingState(), "state");
		ASSERT_FALSE(command.getSerialConsistencyLevel().second);
		ASSERT_FALSE(command.getDefaultTimeStamp().second);
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
		ASSERT_EQ(command.getConsistencyLevel(), cql::CqlConsistencyLevel::Two);
		ASSERT_EQ(value.getFlags(),
			cql::CqlQueryParametersFlags::WithValues |
			cql::CqlQueryParametersFlags::WithSerialConsistency |
			cql::CqlQueryParametersFlags::WithDefaultTimestamp);
		ASSERT_EQ(command.getParameterCount(), 1);
		ASSERT_EQ(command.getParameters(), makeTestString("\x00\x00\x00\x01""a"));
		ASSERT_FALSE(command.getPageSize().second);
		ASSERT_EQ(command.getPagingState(), "");
		ASSERT_EQ(command.getSerialConsistencyLevel().first,
			cql::CqlConsistencyLevel::LocalSerial);
		ASSERT_TRUE(command.getSerialConsistencyLevel().second);
		ASSERT_EQ(command.getDefaultTimeStamp().first,
			std::chrono::system_clock::from_time_t(0) + std::chrono::microseconds(123));
		ASSERT_TRUE(command.getDefaultTimeStamp().second);
	}
}

