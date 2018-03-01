#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlVarInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlVarInt, getset) {
	cql::CqlVarInt value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff'0000'ffff'0000);
	ASSERT_EQ(value.get(), 0x7fff'0000'ffff'0000);

	value = cql::CqlVarInt(-0x8000'0000'0000'0000);
	ASSERT_EQ(value.get(), -0x8000'0000'0000'0000);
}

TEST(TestCqlVarInt, encodeBody) {
	{
		cql::CqlVarInt value;
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00"));
	}
	{
		cql::CqlVarInt value(127);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x7f"));
	}
	{
		cql::CqlVarInt value(128);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x80"));
	}
	{
		cql::CqlVarInt value(-1);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\xff"));
	}
	{
		cql::CqlVarInt value(-128);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x80"));
	}
	{
		cql::CqlVarInt value(-129);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\xff\x7f"));
	}
	{
		cql::CqlVarInt value(0x1234'5678'abcd'dcba);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba"));
	}
}

TEST(TestCqlVarInt, decodeBody) {
	{
		cql::CqlVarInt value;
		auto data = makeTestString("\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
	{
		cql::CqlVarInt value;
		auto data = makeTestString("\x7f");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 127);
	}
	{
		cql::CqlVarInt value;
		auto data = makeTestString("\x00\x80");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 128);
	}
	{
		cql::CqlVarInt value;
		auto data = makeTestString("\xff");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), -1);
	}
	{
		cql::CqlVarInt value;
		auto data = makeTestString("\x80");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), -128);
	}
	{
		cql::CqlVarInt value;
		auto data = makeTestString("\xff\x7f");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), -129);
	}
	{
		cql::CqlVarInt value;
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x1234'5678'abcd'dcba);
	}
	{
		cql::CqlVarInt value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestCqlVarInt, decodeBodyError) {
	{
		cql::CqlVarInt value(0);
		auto data = makeTestString("\x12\x34\x56\x78\xab\xcd\xdc\xba\x00");
		ASSERT_THROWS(cql::CqlDecodeException, value.decodeBody(data.data(), data.size()));
	}
}

