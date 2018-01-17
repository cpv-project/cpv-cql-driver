#include <LowLevel/ProtocolTypes/CqlProtocolQueryParameters.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolQueryParameters, getset) {
	{
		cql::CqlProtocolQueryParameters value;
		value.setConsistency(cql::CqlConsistencyLevel::One);
		value.setSkipMetadata(true);
		value.setValues({ cql::CqlProtocolValue("a"), cql::CqlProtocolValue("b") });
		value.setPageSize(100);
		value.setPagingState("state");
		ASSERT_EQ(value.getConsistency(), cql::CqlConsistencyLevel::One);
		ASSERT_EQ(value.getFlags(),
			cql::CqlQueryParametersFlags::SkipMetadata |
			cql::CqlQueryParametersFlags::WithValues |
			cql::CqlQueryParametersFlags::WithPageSize |
			cql::CqlQueryParametersFlags::WithPagingState);
		ASSERT_EQ(value.getValues().size(), 2);
		ASSERT_EQ(value.getValues().at(0).get(), "a");
		ASSERT_EQ(value.getValues().at(1).get(), "b");
		ASSERT_EQ(value.getPageSize(), 100);
		ASSERT_EQ(value.getPagingState(), "state");
	}
	{
		cql::CqlProtocolQueryParameters value;
		value.setConsistency(cql::CqlConsistencyLevel::Two);
		value.setSkipMetadata(false);
		value.setNameAndValues(
			{ cql::CqlProtocolString("q"), cql::CqlProtocolString("w") },
			{ cql::CqlProtocolValue("a"), cql::CqlProtocolValue("b") });
		value.setSerialConsistency(cql::CqlConsistencyLevel::LocalSerial);
		value.setDefaultTimestamp(123);
		ASSERT_EQ(value.getConsistency(), cql::CqlConsistencyLevel::Two);
		ASSERT_EQ(value.getFlags(),
			cql::CqlQueryParametersFlags::WithValues |
			cql::CqlQueryParametersFlags::WithNamesForValue |
			cql::CqlQueryParametersFlags::WithSerialConsistency |
			cql::CqlQueryParametersFlags::WithDefaultTimestamp);
		ASSERT_EQ(value.getNames().size(), 2);
		ASSERT_EQ(value.getNames().at(0).get(), "q");
		ASSERT_EQ(value.getNames().at(1).get(), "w");
		ASSERT_EQ(value.getValues().size(), 2);
		ASSERT_EQ(value.getValues().at(0).get(), "a");
		ASSERT_EQ(value.getValues().at(1).get(), "b");
		ASSERT_EQ(value.getSerialConsistency(), cql::CqlConsistencyLevel::LocalSerial);
		ASSERT_EQ(value.getDefaultTimestamp(), 123);
	}
}

TEST(TestCqlProtocolQueryParameters, encode) {
	{
		cql::CqlProtocolQueryParameters value;
		value.setConsistency(cql::CqlConsistencyLevel::One);
		value.setSkipMetadata(true);
		value.setValues({ cql::CqlProtocolValue("a"), cql::CqlProtocolValue("b") });
		value.setPageSize(100);
		value.setPagingState("state");
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
		value.setConsistency(cql::CqlConsistencyLevel::Two);
		value.setSkipMetadata(false);
		value.setNameAndValues(
			{ cql::CqlProtocolString("q"), cql::CqlProtocolString("w") },
			{ cql::CqlProtocolValue("a"), cql::CqlProtocolValue("b") });
		value.setSerialConsistency(cql::CqlConsistencyLevel::LocalSerial);
		value.setDefaultTimestamp(123);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x02"
			"\x71"
			"\x00\x02"
			"\x00\x01""q"
			"\x00\x00\x00\x01""a"
			"\x00\x01""w"
			"\x00\x00\x00\x01""b"
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
		ASSERT_EQ(value.getConsistency(), cql::CqlConsistencyLevel::One);
		ASSERT_EQ(value.getFlags(),
			cql::CqlQueryParametersFlags::SkipMetadata |
			cql::CqlQueryParametersFlags::WithValues |
			cql::CqlQueryParametersFlags::WithPageSize |
			cql::CqlQueryParametersFlags::WithPagingState);
		ASSERT_EQ(value.getValues().size(), 2);
		ASSERT_EQ(value.getValues().at(0).get(), "a");
		ASSERT_EQ(value.getValues().at(1).get(), "b");
		ASSERT_EQ(value.getPageSize(), 100);
		ASSERT_EQ(value.getPagingState(), "state");
	}
	{
		auto data = makeTestString(
			"\x00\x02"
			"\x71"
			"\x00\x02"
			"\x00\x01""q"
			"\x00\x00\x00\x01""a"
			"\x00\x01""w"
			"\x00\x00\x00\x01""b"
			"\x00\x09"
			"\x00\x00\x00\x00\x00\x00\x00\x7b");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getConsistency(), cql::CqlConsistencyLevel::Two);
		ASSERT_EQ(value.getFlags(),
			cql::CqlQueryParametersFlags::WithValues |
			cql::CqlQueryParametersFlags::WithNamesForValue |
			cql::CqlQueryParametersFlags::WithSerialConsistency |
			cql::CqlQueryParametersFlags::WithDefaultTimestamp);
		ASSERT_EQ(value.getNames().size(), 2);
		ASSERT_EQ(value.getNames().at(0).get(), "q");
		ASSERT_EQ(value.getNames().at(1).get(), "w");
		ASSERT_EQ(value.getValues().size(), 2);
		ASSERT_EQ(value.getValues().at(0).get(), "a");
		ASSERT_EQ(value.getValues().at(1).get(), "b");
		ASSERT_EQ(value.getSerialConsistency(), cql::CqlConsistencyLevel::LocalSerial);
		ASSERT_EQ(value.getDefaultTimestamp(), 123);
	}
}

