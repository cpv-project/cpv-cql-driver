#include <CqlDriver/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolLongString.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolLongString, getset) {
	cql::CqlProtocolLongString value("abc");
	ASSERT_EQ(value.get(), "abc");
	value.get().append("aaaaa", 5);
	ASSERT_EQ(value.get(), "abcaaaaa");

	value = cql::CqlProtocolLongString("");
	ASSERT_EQ(value.get(), "");
}

TEST(TestCqlProtocolLongString, encode) {
	cql::CqlProtocolLongString value("abc");
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, seastar::sstring("\x00\x00\x00\x03""abc", 7));

	value = cql::CqlProtocolLongString("");
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, seastar::sstring("\x00\x00\x00\x00", 4));
}

TEST(TestCqlProtocolLongString, decode) {
	cql::CqlProtocolLongString value("");
	{
		seastar::sstring data("\x00\x00\x00\x03""abc", 7);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_EQ(value.get(), "abc");
	}
	{
		seastar::sstring data("\x00\x00\x00\x02""ab", 6);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_EQ(value.get(), "ab");
	}
	{
		seastar::sstring data("\x00\x00\x00\x00", 4);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "");
	}
}

TEST(TestCqlProtocolLongString, decodeError) {
	{
		cql::CqlProtocolLongString value("");
		seastar::sstring data("\x00", 1);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolLongString value("");
		seastar::sstring data("\x00\x00\x00\x02""a", 5);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolLongString value("");
		seastar::sstring data("\xff\xff\xff\xff", 4);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

