#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolLongString.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolLongString, getset) {
	cql::ProtocolLongString value("abc");
	ASSERT_EQ(value.get(), "abc");
	value.get().append("aaaaa", 5);
	ASSERT_EQ(value.get(), "abcaaaaa");

	value = cql::ProtocolLongString("");
	ASSERT_EQ(value.get(), "");
}

TEST(TestProtocolLongString, encode) {
	cql::ProtocolLongString value("abc");
	std::string data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x00\x00\x03""abc"));

	value = cql::ProtocolLongString("");
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));
}

TEST(TestProtocolLongString, decode) {
	cql::ProtocolLongString value("");
	{
		auto data = makeTestString("\x00\x00\x00\x03""abc");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "abc");
	}
	{
		auto data = makeTestString("\x00\x00\x00\x02""ab");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "ab");
	}
	{
		auto data = makeTestString("\x00\x00\x00\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "");
	}
}

TEST(TestProtocolLongString, decodeError) {
	{
		cql::ProtocolLongString value("");
		auto data = makeTestString("\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolLongString value("");
		auto data = makeTestString("\x00\x00\x00\x02""a");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolLongString value("");
		auto data = makeTestString("\xff\xff\xff\xff");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

