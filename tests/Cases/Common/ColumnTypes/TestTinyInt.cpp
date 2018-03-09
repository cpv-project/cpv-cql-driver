#include <CQLDriver/Common/ColumnTypes/TinyInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestTinyInt, getset) {
	cql::TinyInt value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7f);
	ASSERT_EQ(value.get(), 0x7f);

	value = cql::TinyInt(-0x80);
	ASSERT_EQ(value.get(), -0x80);
}

TEST(TestTinyInt, encodeBody) {
	cql::TinyInt value(0x12);
	seastar::sstring data;
	value.encodeBody(data);
	ASSERT_EQ(data, makeTestString("\x12"));
}

TEST(TestTinyInt, decodeBody) {
	{
		cql::TinyInt value(0);
		auto data = makeTestString("\x12");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x12);
	}
	{
		cql::TinyInt value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestTinyInt, decodeBodyError) {
	{
		cql::TinyInt value(0);
		auto data = makeTestString("\x12\x00");
		ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
	}
}

