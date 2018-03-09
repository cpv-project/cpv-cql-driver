#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolValue.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolValue, getset) {
	cql::ProtocolValue value("abc");
	ASSERT_EQ(value.get(), "abc");
	value.get().append("aaaaa", 5);
	ASSERT_EQ(value.get(), "abcaaaaa");
	ASSERT_EQ(value.state(), cql::ProtocolValueState::Normal);
	ASSERT_FALSE(value.isNull());
	ASSERT_FALSE(value.isNotSet());

	value = cql::ProtocolValue(cql::ProtocolValueState::NotSet);
	ASSERT_EQ(value.get(), "");
	ASSERT_EQ(value.state(), cql::ProtocolValueState::NotSet);
	ASSERT_FALSE(value.isNull());
	ASSERT_TRUE(value.isNotSet());

	value = cql::ProtocolValue(cql::ProtocolValueState::Null);
	ASSERT_EQ(value.get(), "");
	ASSERT_EQ(value.state(), cql::ProtocolValueState::Null);
	ASSERT_TRUE(value.isNull());
	ASSERT_FALSE(value.isNotSet());
}

TEST(TestProtocolValue, encode) {
	cql::ProtocolValue value("abc");
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x00\x00\x03""abc"));

	value = cql::ProtocolValue("");
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));

	value = cql::ProtocolValue(cql::ProtocolValueState::NotSet);
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\xff\xff\xff\xfe"));

	value = cql::ProtocolValue(cql::ProtocolValueState::Null);
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\xff\xff\xff\xff"));
}

TEST(TestProtocolValue, decode) {
	cql::ProtocolValue value("");
	{
		auto data = makeTestString("\x00\x00\x00\x03""abc");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "abc");
		ASSERT_EQ(value.state(), cql::ProtocolValueState::Normal);
	}
	{
		auto data = makeTestString("\x00\x00\x00\x02""ab");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "ab");
		ASSERT_EQ(value.state(), cql::ProtocolValueState::Normal);
	}
	{
		auto data = makeTestString("\xff\xff\xff\xfe");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "");
		ASSERT_EQ(value.state(), cql::ProtocolValueState::NotSet);
	}
	{
		auto data = makeTestString("\xff\xff\xff\xff");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "");
		ASSERT_EQ(value.state(), cql::ProtocolValueState::Null);
	}
	{
		auto data = makeTestString("\x00\x00\x00\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "");
		ASSERT_EQ(value.state(), cql::ProtocolValueState::Normal);
	}
}

TEST(TestProtocolValue, decodeError) {
	{
		cql::ProtocolValue value("");
		auto data = makeTestString("\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolValue value("");
		auto data = makeTestString("\x00\x00\x00\x02""a");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolValue value("");
		auto data = makeTestString("\xff\xff\xff\xfd");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

