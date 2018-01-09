#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolVint.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolVint, all) { // TODO: rename to getset
	cql::CqlProtocolVint value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff0000aaaaeeee);
	ASSERT_EQ(value.get(), 0x7fff0000aaaaeeee);

	value = cql::CqlProtocolVint(-3);
	ASSERT_EQ(value.get(), -3);
}

TEST(TestCqlProtocolVint, encode) {
	{
		cql::CqlProtocolVint value(0x7fff0000aaaaeeee);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\xff\xff\xfe\x00\x01\x55\x55\xdd\xdc", 9));
	}
	{
		cql::CqlProtocolVint value(3);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\x06", 1));
	}
	{
		cql::CqlProtocolVint value(-3);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\x05", 1));
	}
	{
		cql::CqlProtocolVint value(-0x7f238a);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\xe0\xfe\x47\x13", 4));
	}
}

TEST(TestCqlProtocolVint, decode) {
	cql::CqlProtocolVint value(0);
	{
		seastar::sstring data("\xff\xff\xfe\x00\x01\x55\x55\xdd\xdc", 9);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 0x7fff0000aaaaeeee);
	}
	{
		seastar::sstring data("\x06", 1);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 3);
	}
	{
		seastar::sstring data("\x05", 1);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), -3);
	}
	{
		seastar::sstring data("\xe0\xfe\x47\x13", 4);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), -0x7f238a);
	}
}

TEST(TestCqlProtocolVint, decodeError) {
	{
		cql::CqlProtocolVint value(0);
		seastar::sstring data;
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolVint value(0);
		seastar::sstring data("\xe0\xfe\x47", 3);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

