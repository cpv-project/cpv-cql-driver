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

TEST(TestCqlTinyInt, encode) {
	cql::CqlTinyInt value(0x12);
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x12"));
}

TEST(TestCqlTinyInt, decode) {
	{
		cql::CqlTinyInt value(0);
		auto data = makeTestString("\x12");
		value.decode(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x12);
	}
	{
		cql::CqlTinyInt value(123);
		auto data = makeTestString("");
		value.decode(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestCqlTinyInt, decodeError) {
	{
		cql::CqlTinyInt value(0);
		auto data = makeTestString("\x12\x00");
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(data.data(), data.size()));
	}
}

