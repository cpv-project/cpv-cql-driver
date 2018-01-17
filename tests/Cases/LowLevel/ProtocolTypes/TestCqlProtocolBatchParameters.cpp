#include <LowLevel/ProtocolTypes/CqlProtocolBatchParameters.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolBatchParameters, getset) {
	{
		cql::CqlProtocolBatchParameters value;
		value.setConsistency(cql::CqlConsistencyLevel::One);
		ASSERT_EQ(value.getConsistency(), cql::CqlConsistencyLevel::One);
		ASSERT_EQ(value.getFlags(), cql::CqlBatchParametersFlags::None);
	}
	{
		cql::CqlProtocolBatchParameters value;
		value.setConsistency(cql::CqlConsistencyLevel::Two);
		value.setSerialConsistency(cql::CqlConsistencyLevel::LocalSerial);
		value.setDefaultTimestamp(123);
		ASSERT_EQ(value.getConsistency(), cql::CqlConsistencyLevel::Two);
		ASSERT_EQ(value.getFlags(),
			cql::CqlBatchParametersFlags::WithSerialConsistency |
			cql::CqlBatchParametersFlags::WithDefaultTimestamp);
		ASSERT_EQ(value.getSerialConsistency(), cql::CqlConsistencyLevel::LocalSerial);
		ASSERT_EQ(value.getDefaultTimestamp(), 123);
	}
}

TEST(TestCqlProtocolBatchParameters, encode) {
	{
		cql::CqlProtocolBatchParameters value;
		value.setConsistency(cql::CqlConsistencyLevel::One);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x01"
			"\x00"));
	}
	{
		cql::CqlProtocolBatchParameters value;
		value.setConsistency(cql::CqlConsistencyLevel::Two);
		value.setSerialConsistency(cql::CqlConsistencyLevel::LocalSerial);
		value.setDefaultTimestamp(123);
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
		ASSERT_EQ(value.getConsistency(), cql::CqlConsistencyLevel::One);
		ASSERT_EQ(value.getFlags(), cql::CqlBatchParametersFlags::None);
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
		ASSERT_EQ(value.getConsistency(), cql::CqlConsistencyLevel::Two);
		ASSERT_EQ(value.getFlags(),
			cql::CqlBatchParametersFlags::WithSerialConsistency |
			cql::CqlBatchParametersFlags::WithDefaultTimestamp);
		ASSERT_EQ(value.getSerialConsistency(), cql::CqlConsistencyLevel::LocalSerial);
		ASSERT_EQ(value.getDefaultTimestamp(), 123);
	}
}

