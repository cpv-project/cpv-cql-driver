#include <CQLDriver/Common/ColumnTypes/BigInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestBigInt, getset) {
	cql::BigInt value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff'0000'ffff'0000);
	ASSERT_EQ(value.get(), 0x7fff'0000'ffff'0000);

	value = cql::BigInt(-0x8000'0000'0000'0000);
	ASSERT_EQ(value.get(), static_cast<std::int64_t>(-0x8000'0000'0000'0000));
}

TEST(TestBigInt, encodeBody) {
	cql::BigInt value(0x1234'5678'abcd'dcba);
	std::string data;
	value.encodeBody(data);
	ASSERT_EQ(data, makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba"));
}

TEST(TestBigInt, decodeBody) {
	{
		cql::BigInt value(0);
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x1234'5678'abcd'dcba);
	}
	{
		cql::BigInt value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestBigInt, decodeBodyError) {
	{
		cql::BigInt value(0);
		auto data = makeTestString("\x12");
		ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
	}
	{
		cql::BigInt value(0);
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba\x00");
		ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
	}
}

