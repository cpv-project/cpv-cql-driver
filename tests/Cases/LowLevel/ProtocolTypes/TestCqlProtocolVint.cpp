#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolVint.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolVint, getset) {
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
		ASSERT_EQ(data, makeTestString("\xff\xff\xfe\x00\x01\x55\x55\xdd\xdc"));
	}
	{
		cql::CqlProtocolVint value(3);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x06"));
	}
	{
		cql::CqlProtocolVint value(-3);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x05"));
	}
	{
		cql::CqlProtocolVint value(-0x7f238a);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\xe0\xfe\x47\x13"));
	}
}

TEST(TestCqlProtocolVint, decode) {
	cql::CqlProtocolVint value(0);
	{
		auto data = makeTestString("\xff\xff\xfe\x00\x01\x55\x55\xdd\xdc");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 0x7fff0000aaaaeeee);
	}
	{
		auto data = makeTestString("\x06");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 3);
	}
	{
		auto data = makeTestString("\x05");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), -3);
	}
	{
		auto data = makeTestString("\xe0\xfe\x47\x13");
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
		auto data = makeTestString("\xe0\xfe\x47");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

