#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlTinyInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlTinyInt, getset) {
	cql::CqlTinyInt value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7f);
	ASSERT_EQ(value.get(), 0x7f);

	value = cql::CqlTinyInt(-0x80);
	ASSERT_EQ(value.get(), -0x80);
}

TEST(TestCqlTinyInt, encodeBody) {
	cql::CqlTinyInt value(0x12);
	seastar::sstring data;
	value.encodeBody(data);
	ASSERT_EQ(data, makeTestString("\x12"));
}

TEST(TestCqlTinyInt, decodeBody) {
	{
		cql::CqlTinyInt value(0);
		auto data = makeTestString("\x12");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x12);
	}
	{
		cql::CqlTinyInt value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestCqlTinyInt, decodeBodyError) {
	{
		cql::CqlTinyInt value(0);
		auto data = makeTestString("\x12\x00");
		ASSERT_THROWS(cql::CqlDecodeException, value.decodeBody(data.data(), data.size()));
	}
}

