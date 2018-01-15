#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolBytes.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolBytes, getset) {
	{
		cql::CqlProtocolBytes value("abc");
		ASSERT_EQ(value.get(), "abc");
		value.get().append("aaaaa", 5);
		ASSERT_EQ(value.get(), "abcaaaaa");
		ASSERT_FALSE(value.isNull());

		value = cql::CqlProtocolBytes();
		ASSERT_EQ(value.get(), "");
		ASSERT_TRUE(value.isNull());
	}
	{
		cql::CqlProtocolBytes value("abc");
		value.set(cql::CqlProtocolBytesState::Null);
		ASSERT_EQ(value.get(), "");
		ASSERT_TRUE(value.isNull());

		value.set("asdfg", 5);
		ASSERT_EQ(value.get(), "asdfg");
		ASSERT_FALSE(value.isNull());

		value.set(cql::CqlProtocolBytesState::Null);
		value.append("asd", 3);
		ASSERT_EQ(value.get(), "asd");
		ASSERT_FALSE(value.isNull());
	}
}

TEST(TestCqlProtocolBytes, encode) {
	cql::CqlProtocolBytes value("abc");
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x00\x00\x03""abc"));

	value = cql::CqlProtocolBytes("");
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));

	value = cql::CqlProtocolBytes();
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\xff\xff\xff\xff"));
}

TEST(TestCqlProtocolBytes, decode) {
	cql::CqlProtocolBytes value;
	{
		auto data = makeTestString("\x00\x00\x00\x03""abc");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "abc");
		ASSERT_FALSE(value.isNull());
	}
	{
		auto data = makeTestString("\x00\x00\x00\x02""ab");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "ab");
		ASSERT_FALSE(value.isNull());
	}
	{
		auto data = makeTestString("\xff\xff\xff\xff");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "");
		ASSERT_TRUE(value.isNull());
	}
	{
		auto data = makeTestString("\x00\x00\x00\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "");
		ASSERT_FALSE(value.isNull());
	}
}

TEST(TestCqlProtocolBytes, decodeError) {
	{
		cql::CqlProtocolBytes value;
		auto data = makeTestString("\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolBytes value;
		auto data = makeTestString("\x00\x00\x00\x02""a");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

