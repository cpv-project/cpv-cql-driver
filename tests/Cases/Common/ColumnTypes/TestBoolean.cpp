#include <CQLDriver/Common/ColumnTypes/Boolean.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestBoolean, getset) {
	cql::Boolean value(false);
	ASSERT_EQ(value.get(), false);
	value.set(true);
	ASSERT_EQ(value.get(), true);

	value = cql::Boolean();
	ASSERT_EQ(value.get(), false);
}

TEST(TestBoolean, encodeBody) {
	{
		cql::Boolean value(true);
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x01"));
	}
	{
		cql::Boolean value(false);
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00"));
	}
}

TEST(TestBoolean, decodeBody) {
	{
		cql::Boolean value(true);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), false);
	}
	{
		cql::Boolean value(true);
		auto data = makeTestString("\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), false);
	}
	{
		cql::Boolean value(false);
		auto data = makeTestString("\x01");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), true);
	}
}

TEST(TestBoolean, decodeBodyError) {
	{
		cql::Boolean value(0);
		auto data = makeTestString("\x01\x00");
		ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
	}
}

