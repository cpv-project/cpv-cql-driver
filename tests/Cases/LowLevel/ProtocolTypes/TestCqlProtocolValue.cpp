#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolValue.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolValue, getset) {
	cql::CqlProtocolValue value("abc");
	ASSERT_EQ(value.get(), "abc");
	value.get().append("aaaaa", 5);
	ASSERT_EQ(value.get(), "abcaaaaa");
	ASSERT_EQ(value.state(), cql::CqlProtocolValueState::Normal);
	ASSERT_FALSE(value.isNull());
	ASSERT_FALSE(value.isNotSet());

	value = cql::CqlProtocolValue(cql::CqlProtocolValueState::NotSet);
	ASSERT_EQ(value.get(), "");
	ASSERT_EQ(value.state(), cql::CqlProtocolValueState::NotSet);
	ASSERT_FALSE(value.isNull());
	ASSERT_TRUE(value.isNotSet());

	value = cql::CqlProtocolValue(cql::CqlProtocolValueState::Null);
	ASSERT_EQ(value.get(), "");
	ASSERT_EQ(value.state(), cql::CqlProtocolValueState::Null);
	ASSERT_TRUE(value.isNull());
	ASSERT_FALSE(value.isNotSet());
}

TEST(TestCqlProtocolValue, encode) {
	cql::CqlProtocolValue value("abc");
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x00\x00\x03""abc"));

	value = cql::CqlProtocolValue("");
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));

	value = cql::CqlProtocolValue(cql::CqlProtocolValueState::NotSet);
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\xff\xff\xff\xfe"));

	value = cql::CqlProtocolValue(cql::CqlProtocolValueState::Null);
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\xff\xff\xff\xff"));
}

TEST(TestCqlProtocolValue, decode) {
	cql::CqlProtocolValue value("");
	{
		auto data = makeTestString("\x00\x00\x00\x03""abc");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "abc");
		ASSERT_EQ(value.state(), cql::CqlProtocolValueState::Normal);
	}
	{
		auto data = makeTestString("\x00\x00\x00\x02""ab");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "ab");
		ASSERT_EQ(value.state(), cql::CqlProtocolValueState::Normal);
	}
	{
		auto data = makeTestString("\xff\xff\xff\xfe");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "");
		ASSERT_EQ(value.state(), cql::CqlProtocolValueState::NotSet);
	}
	{
		auto data = makeTestString("\xff\xff\xff\xff");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "");
		ASSERT_EQ(value.state(), cql::CqlProtocolValueState::Null);
	}
	{
		auto data = makeTestString("\x00\x00\x00\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "");
		ASSERT_EQ(value.state(), cql::CqlProtocolValueState::Normal);
	}
}

TEST(TestCqlProtocolValue, decodeError) {
	{
		cql::CqlProtocolValue value("");
		auto data = makeTestString("\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolValue value("");
		auto data = makeTestString("\x00\x00\x00\x02""a");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolValue value("");
		auto data = makeTestString("\xff\xff\xff\xfd");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

