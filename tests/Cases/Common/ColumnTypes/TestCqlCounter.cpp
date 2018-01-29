#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlCounter.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlCounter, getset) {
	cql::CqlCounter value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff'0000'ffff'0000);
	ASSERT_EQ(value.get(), 0x7fff'0000'ffff'0000);

	value = cql::CqlCounter(-0x8000'0000'0000'0000);
	ASSERT_EQ(value.get(), -0x8000'0000'0000'0000);
}

TEST(TestCqlCounter, encode) {
	cql::CqlCounter value(0x1234'5678'abcd'dcba);
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba"));
}

TEST(TestCqlCounter, decode) {
	{
		cql::CqlCounter value(0);
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba");
		value.decode(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x1234'5678'abcd'dcba);
	}
	{
		cql::CqlCounter value(123);
		auto data = makeTestString("");
		value.decode(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestCqlCounter, decodeError) {
	{
		cql::CqlCounter value(0);
		auto data = makeTestString("\x12");
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(data.data(), data.size()));
	}
	{
		cql::CqlCounter value(0);
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba\x00");
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(data.data(), data.size()));
	}
}

