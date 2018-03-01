#include <CqlDriver/Common/ColumnTypes/CqlSmallInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlSmallInt, getset) {
	cql::CqlSmallInt value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff);
	ASSERT_EQ(value.get(), 0x7fff);

	value = cql::CqlSmallInt(-0x8000);
	ASSERT_EQ(value.get(), -0x8000);
}

TEST(TestCqlSmallInt, encodeBody) {
	cql::CqlSmallInt value(0x1234);
	seastar::sstring data;
	value.encodeBody(data);
	ASSERT_EQ(data, makeTestString("\x12\x34"));
}

TEST(TestCqlSmallInt, decodeBody) {
	{
		cql::CqlSmallInt value(0);
		auto data = makeTestString("\x12\x34");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x1234);
	}
	{
		cql::CqlSmallInt value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestCqlSmallInt, decodeBodyError) {
	{
		cql::CqlSmallInt value(0);
		auto data = makeTestString("\x12");
		ASSERT_THROWS(cql::CqlDecodeException, value.decodeBody(data.data(), data.size()));
	}
	{
		cql::CqlSmallInt value(0);
		auto data = makeTestString("\x12\x34\x00");
		ASSERT_THROWS(cql::CqlDecodeException, value.decodeBody(data.data(), data.size()));
	}
}

