#include <CQLDriver/Common/ColumnTypes/VarInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestVarInt, getset) {
	cql::VarInt value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff'0000'ffff'0000);
	ASSERT_EQ(value.get(), 0x7fff'0000'ffff'0000);

	value = cql::VarInt(-0x8000'0000'0000'0000);
	ASSERT_EQ(value.get(), -0x8000'0000'0000'0000);
}

TEST(TestVarInt, encodeBody) {
	{
		cql::VarInt value;
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00"));
	}
	{
		cql::VarInt value(127);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x7f"));
	}
	{
		cql::VarInt value(128);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x80"));
	}
	{
		cql::VarInt value(-1);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\xff"));
	}
	{
		cql::VarInt value(-128);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x80"));
	}
	{
		cql::VarInt value(-129);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\xff\x7f"));
	}
	{
		cql::VarInt value(0x1234'5678'abcd'dcba);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba"));
	}
}

TEST(TestVarInt, decodeBody) {
	{
		cql::VarInt value;
		auto data = makeTestString("\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
	{
		cql::VarInt value;
		auto data = makeTestString("\x7f");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 127);
	}
	{
		cql::VarInt value;
		auto data = makeTestString("\x00\x80");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 128);
	}
	{
		cql::VarInt value;
		auto data = makeTestString("\xff");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), -1);
	}
	{
		cql::VarInt value;
		auto data = makeTestString("\x80");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), -128);
	}
	{
		cql::VarInt value;
		auto data = makeTestString("\xff\x7f");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), -129);
	}
	{
		cql::VarInt value;
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x1234'5678'abcd'dcba);
	}
	{
		cql::VarInt value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestVarInt, decodeBodyError) {
	{
		cql::VarInt value(0);
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba\x00");
		ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
	}
}

