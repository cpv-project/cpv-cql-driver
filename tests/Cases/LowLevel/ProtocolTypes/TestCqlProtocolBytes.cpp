#include <CqlDriver/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolBytes.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolBytes, getset) {
	cql::CqlProtocolBytes value("abc");
	ASSERT_EQ(value.get(), "abc");
	value.get().append("aaaaa", 5);
	ASSERT_EQ(value.get(), "abcaaaaa");
	ASSERT_FALSE(value.isNull());

	value = cql::CqlProtocolBytes();
	ASSERT_EQ(value.get(), "");
	ASSERT_TRUE(value.isNull());
}

TEST(TestCqlProtocolBytes, encode) {
	cql::CqlProtocolBytes value("abc");
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, seastar::sstring("\x00\x00\x00\x03""abc", 7));

	value = cql::CqlProtocolBytes("");
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, seastar::sstring("\x00\x00\x00\x00", 4));

	value = cql::CqlProtocolBytes();
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, seastar::sstring("\xff\xff\xff\xff"));
}

TEST(TestCqlProtocolBytes, decode) {
	cql::CqlProtocolBytes value;
	{
		seastar::sstring data("\x00\x00\x00\x03""abc", 7);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "abc");
		ASSERT_FALSE(value.isNull());
	}
	{
		seastar::sstring data("\x00\x00\x00\x02""ab", 6);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "ab");
		ASSERT_FALSE(value.isNull());
	}
	{
		seastar::sstring data("\xff\xff\xff\xff");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "");
		ASSERT_TRUE(value.isNull());
	}
	{
		seastar::sstring data("\x00\x00\x00\x00", 4);
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
		seastar::sstring data("\x00", 1);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolBytes value;
		seastar::sstring data("\x00\x00\x00\x02""a", 5);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

