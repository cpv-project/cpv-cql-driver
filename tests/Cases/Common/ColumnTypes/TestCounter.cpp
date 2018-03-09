#include <CQLDriver/Common/ColumnTypes/Counter.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCounter, getset) {
	cql::Counter value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff'0000'ffff'0000);
	ASSERT_EQ(value.get(), 0x7fff'0000'ffff'0000);

	value = cql::Counter(-0x8000'0000'0000'0000);
	ASSERT_EQ(value.get(), -0x8000'0000'0000'0000);
}

TEST(TestCounter, encodeBody) {
	cql::Counter value(0x1234'5678'abcd'dcba);
	seastar::sstring data;
	value.encodeBody(data);
	ASSERT_EQ(data, makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba"));
}

TEST(TestCounter, decodeBody) {
	{
		cql::Counter value(0);
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x1234'5678'abcd'dcba);
	}
	{
		cql::Counter value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestCounter, decodeBodyError) {
	{
		cql::Counter value(0);
		auto data = makeTestString("\x12");
		ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
	}
	{
		cql::Counter value(0);
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba\x00");
		ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
	}
}

