#include <CqlDriver/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolShortBytes.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolShortBytes, getset) {
	cql::CqlProtocolShortBytes value("abc");
	ASSERT_EQ(value.get(), "abc");
	value.get().append("aaaaa", 5);
	ASSERT_EQ(value.get(), "abcaaaaa");

	value = cql::CqlProtocolShortBytes("");
	ASSERT_EQ(value.get(), "");
}

TEST(TestCqlProtocolShortBytes, encode) {
	cql::CqlProtocolShortBytes value("abc");
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, seastar::sstring("\x00\x03""abc", 5));

	value = cql::CqlProtocolShortBytes("");
	data.resize(0);
	value.encode(data);
	ASSERT_EQ(data, seastar::sstring("\x00\x00", 2));
}

TEST(TestCqlProtocolShortBytes, decode) {
	cql::CqlProtocolShortBytes value("");
	{
		seastar::sstring data("\x00\x03""abc", 5);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_EQ(value.get(), "abc");
	}
	{
		seastar::sstring data("\x00\x02""ab", 4);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_EQ(value.get(), "ab");
	}
	{
		seastar::sstring data("\x00\x00", 2);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), "");
	}
}

TEST(TestCqlProtocolShortBytes, decodeError) {
	{
		cql::CqlProtocolShortBytes value("");
		seastar::sstring data("\x00", 1);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolShortBytes value("");
		seastar::sstring data("\x00\x02""a", 3);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

