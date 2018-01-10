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
		ASSERT_EQ(data, seastar::sstring("\xff\x7f\xff\x00\x00\xaa\xaa\xee\xee", 9));
	}
	{
		cql::CqlProtocolUnsignedVint value(3);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\x03", 1));
	}
	{
		cql::CqlProtocolUnsignedVint value(0x12238a);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\xd2\x23\x8a", 3));
	}
	{
		cql::CqlProtocolUnsignedVint value(0x7f238a);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\xe0\x7f\x23\x8a", 4));
	}
}

TEST(TestCqlProtocolUnsignedVint, decode) {
	cql::CqlProtocolUnsignedVint value(0);
	{
		seastar::sstring data("\xff\x7f\xff\x00\x00\xaa\xaa\xee\xee", 9);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 0x7fff0000aaaaeeee);
	}
	{
		seastar::sstring data("\x03", 1);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 3);
	}
	{
		seastar::sstring data("\xd2\x23\x8a", 3);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 0x12238a);
	}
	{
		seastar::sstring data("\xe0\x7f\x23\x8a", 4);
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
		seastar::sstring data("\xe0\x7f\x23", 3);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

