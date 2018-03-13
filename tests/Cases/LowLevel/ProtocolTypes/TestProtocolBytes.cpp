#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolBytes.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolBytes, getset) {
	{
		cql::ProtocolBytes value("abc");
		ASSERT_EQ(value.get(), "abc");
		value.get().append("aaaaa", 5);
		ASSERT_EQ(value.get(), "abcaaaaa");
		ASSERT_FALSE(value.isNull());

		value = cql::ProtocolBytes();
		ASSERT_EQ(value.get(), "");
		ASSERT_TRUE(value.isNull());
	}
	{
		cql::ProtocolBytes value("abc");
		value.set(cql::ProtocolBytesState::Null);
		ASSERT_EQ(value.get(), "");
		ASSERT_TRUE(value.isNull());

		value.set("asdfg", 5);
		ASSERT_EQ(value.get(), "asdfg");
		ASSERT_FALSE(value.isNull());

		value.set(cql::ProtocolBytesState::Null);
		value.append("asd", 3);
		ASSERT_EQ(value.get(), "asd");
		ASSERT_FALSE(value.isNull());
	}
}

TEST(TestProtocolBytes, encode) {
	cql::ProtocolBytes value("abc");
	std::string data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x00\x00\x03""abc"));

	value = cql::ProtocolBytes("");
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));

	value = cql::ProtocolBytes();
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\xff\xff\xff\xff"));
}

TEST(TestProtocolBytes, decode) {
	cql::ProtocolBytes value;
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

TEST(TestProtocolBytes, decodeError) {
	{
		cql::ProtocolBytes value;
		auto data = makeTestString("\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolBytes value;
		auto data = makeTestString("\x00\x00\x00\x02""a");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

