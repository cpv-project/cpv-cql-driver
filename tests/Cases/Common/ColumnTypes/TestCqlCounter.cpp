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

TEST(TestCqlCounter, encodeBody) {
	cql::CqlCounter value(0x1234'5678'abcd'dcba);
	seastar::sstring data;
	value.encodeBody(data);
	ASSERT_EQ(data, makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba"));
}

TEST(TestCqlCounter, decodeBody) {
	{
		cql::CqlCounter value(0);
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x1234'5678'abcd'dcba);
	}
	{
		cql::CqlCounter value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestCqlCounter, decodeBodyError) {
	{
		cql::CqlCounter value(0);
		auto data = makeTestString("\x12");
		ASSERT_THROWS(cql::CqlDecodeException, value.decodeBody(data.data(), data.size()));
	}
	{
		cql::CqlCounter value(0);
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba\x00");
		ASSERT_THROWS(cql::CqlDecodeException, value.decodeBody(data.data(), data.size()));
	}
}

