#include <CQLDriver/Common/ColumnTypes/SmallInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestSmallInt, getset) {
	cql::SmallInt value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff);
	ASSERT_EQ(value.get(), 0x7fff);

	value = cql::SmallInt(-0x8000);
	ASSERT_EQ(value.get(), -0x8000);
}

TEST(TestSmallInt, encodeBody) {
	cql::SmallInt value(0x1234);
	seastar::sstring data;
	value.encodeBody(data);
	ASSERT_EQ(data, makeTestString("\x12\x34"));
}

TEST(TestSmallInt, decodeBody) {
	{
		cql::SmallInt value(0);
		auto data = makeTestString("\x12\x34");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x1234);
	}
	{
		cql::SmallInt value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestSmallInt, decodeBodyError) {
	{
		cql::SmallInt value(0);
		auto data = makeTestString("\x12");
		ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
	}
	{
		cql::SmallInt value(0);
		auto data = makeTestString("\x12\x34\x00");
		ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
	}
}

