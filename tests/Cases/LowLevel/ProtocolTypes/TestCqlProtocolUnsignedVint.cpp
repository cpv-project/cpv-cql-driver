#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolUnsignedVint.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolUnsignedVint, getset) {
	cql::CqlProtocolUnsignedVint value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff0000aaaaeeee);
	ASSERT_EQ(value.get(), 0x7fff0000aaaaeeee);

	value = cql::CqlProtocolUnsignedVint(0xffff0000aaaaeeee);
	ASSERT_EQ(value.get(), 0xffff0000aaaaeeee);
}

TEST(TestCqlProtocolUnsignedVint, encode) {
	{
		cql::CqlProtocolUnsignedVint value(0x7fff0000aaaaeeee);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\xff\x7f\xff\x00\x00\xaa\xaa\xee\xee"));
	}
	{
		cql::CqlProtocolUnsignedVint value(3);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x03"));
	}
	{
		cql::CqlProtocolUnsignedVint value(0x12238a);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\xd2\x23\x8a"));
	}
	{
		cql::CqlProtocolUnsignedVint value(0x7f238a);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\xe0\x7f\x23\x8a"));
	}
}

TEST(TestCqlProtocolUnsignedVint, decode) {
	cql::CqlProtocolUnsignedVint value(0);
	{
		auto data = makeTestString("\xff\x7f\xff\x00\x00\xaa\xaa\xee\xee");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 0x7fff0000aaaaeeee);
	}
	{
		auto data = makeTestString("\x03");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 3);
	}
	{
		auto data = makeTestString("\xd2\x23\x8a");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 0x12238a);
	}
	{
		auto data = makeTestString("\xe0\x7f\x23\x8a");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 0x7f238a);
	}
}

TEST(TestCqlProtocolUnsignedVint, decodeError) {
	{
		cql::CqlProtocolUnsignedVint value(0);
		seastar::sstring data;
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolUnsignedVint value(0);
		auto data = makeTestString("\xe0\x7f\x23");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

