#include <LowLevel/ProtocolTypes/CqlProtocolBatchParameters.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolBatchParameters, getset) {
	{
		cql::CqlProtocolBatchParameters value;
		value.setBatchCommand(
			cql::CqlBatchCommand()
				.setConsistencyLevel(cql::CqlConsistencyLevel::One));
		ASSERT_EQ(value.getFlags(), cql::CqlBatchParametersFlags::None);
		ASSERT_EQ(value.getBatchCommand().getConsistencyLevel(), cql::CqlConsistencyLevel::One);
	}
	{
		cql::CqlProtocolBatchParameters value;
		value.setBatchCommand(
			cql::CqlBatchCommand()
				.setConsistencyLevel(cql::CqlConsistencyLevel::Two)
				.setSerialConsistencyLevel(cql::CqlConsistencyLevel::LocalSerial)
				.setDefaultTimeStamp(
					std::chrono::system_clock::from_time_t(0) +
					std::chrono::microseconds(123)));
		ASSERT_EQ(value.getFlags(),
			cql::CqlBatchParametersFlags::WithSerialConsistency |
			cql::CqlBatchParametersFlags::WithDefaultTimestamp);
		ASSERT_EQ(value.getBatchCommand().getConsistencyLevel(), cql::CqlConsistencyLevel::Two);
		ASSERT_EQ(value.getBatchCommand().getSerialConsistencyLevel().first,
			cql::CqlConsistencyLevel::LocalSerial);
		ASSERT_TRUE(value.getBatchCommand().getSerialConsistencyLevel().second);
		ASSERT_EQ(value.getBatchCommand().getDefaultTimeStamp().first,
			std::chrono::system_clock::from_time_t(0) + std::chrono::microseconds(123));
		ASSERT_TRUE(value.getBatchCommand().getDefaultTimeStamp().second);
	}
}

TEST(TestCqlProtocolBatchParameters, encode) {
	{
		cql::CqlProtocolBatchParameters value;
		value.setBatchCommand(
			cql::CqlBatchCommand()
				.setConsistencyLevel(cql::CqlConsistencyLevel::One));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x01"
			"\x00"));
	}
	{
		cql::CqlProtocolBatchParameters value;
		value.setBatchCommand(
			cql::CqlBatchCommand()
				.setConsistencyLevel(cql::CqlConsistencyLevel::Two)
				.setSerialConsistencyLevel(cql::CqlConsistencyLevel::LocalSerial)
				.setDefaultTimeStamp(
					std::chrono::system_clock::from_time_t(0) +
					std::chrono::microseconds(123)));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x02"
			"\x30"
			"\x00\x09"
			"\x00\x00\x00\x00\x00\x00\x00\x7b"));
	}
}

TEST(TestCqlProtocolBatchParameters, decode) {
	cql::CqlProtocolBatchParameters value;
	{
		auto data = makeTestString(
			"\x00\x01"
			"\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getFlags(), cql::CqlBatchParametersFlags::None);
		ASSERT_TRUE(value.getBatchCommand().isValid());
		ASSERT_EQ(value.getBatchCommand().getConsistencyLevel(), cql::CqlConsistencyLevel::One);
	}
	{
		auto data = makeTestString(
			"\x00\x02"
			"\x30"
			"\x00\x09"
			"\x00\x00\x00\x00\x00\x00\x00\x7b");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getFlags(),
			cql::CqlBatchParametersFlags::WithSerialConsistency |
			cql::CqlBatchParametersFlags::WithDefaultTimestamp);
		ASSERT_TRUE(value.getBatchCommand().isValid());
		ASSERT_EQ(value.getBatchCommand().getConsistencyLevel(), cql::CqlConsistencyLevel::Two);
		ASSERT_EQ(value.getBatchCommand().getSerialConsistencyLevel().first,
			cql::CqlConsistencyLevel::LocalSerial);
		ASSERT_TRUE(value.getBatchCommand().getSerialConsistencyLevel().second);
		ASSERT_EQ(value.getBatchCommand().getDefaultTimeStamp().first,
			std::chrono::system_clock::from_time_t(0) + std::chrono::microseconds(123));
		ASSERT_TRUE(value.getBatchCommand().getDefaultTimeStamp().second);
	}
}

