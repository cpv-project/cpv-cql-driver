#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlBigInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlBigInt, getset) {
	cql::CqlBigInt value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff'0000'ffff'0000);
	ASSERT_EQ(value.get(), 0x7fff'0000'ffff'0000);

	value = cql::CqlBigInt(-0x8000'0000'0000'0000);
	ASSERT_EQ(value.get(), -0x8000'0000'0000'0000);
}

TEST(TestCqlBigInt, encode) {
	cql::CqlBigInt value(0x1234'5678'abcd'dcba);
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba"));
}

TEST(TestCqlBigInt, decode) {
	{
		cql::CqlBigInt value(0);
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba");
		value.decode(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x1234'5678'abcd'dcba);
	}
	{
		cql::CqlBigInt value(123);
		auto data = makeTestString("");
		value.decode(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestCqlBigInt, decodeError) {
	{
		cql::CqlBigInt value(0);
		auto data = makeTestString("\x12");
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(data.data(), data.size()));
	}
	{
		cql::CqlBigInt value(0);
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba\x00");
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(data.data(), data.size()));
	}
}

