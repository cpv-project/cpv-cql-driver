#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolUnsignedVint.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolUnsignedVint, getset) {
	cql::ProtocolUnsignedVint value(1);
	ASSERT_EQ(value.get(), 1U);
	value.set(0x7fff0000aaaaeeee);
	ASSERT_EQ(value.get(), 0x7fff0000aaaaeeeeU);

	value = cql::ProtocolUnsignedVint(0xffff0000aaaaeeee);
	ASSERT_EQ(value.get(), 0xffff0000aaaaeeeeU);
}

TEST(TestProtocolUnsignedVint, encode) {
	{
		cql::ProtocolUnsignedVint value(0x7fff0000aaaaeeee);
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\xff\x7f\xff\x00\x00\xaa\xaa\xee\xee"));
	}
	{
		cql::ProtocolUnsignedVint value(3);
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x03"));
	}
	{
		cql::ProtocolUnsignedVint value(0x12238a);
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\xd2\x23\x8a"));
	}
	{
		cql::ProtocolUnsignedVint value(0x7f238a);
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\xe0\x7f\x23\x8a"));
	}
}

TEST(TestProtocolUnsignedVint, decode) {
	cql::ProtocolUnsignedVint value(0);
	{
		auto data = makeTestString("\xff\x7f\xff\x00\x00\xaa\xaa\xee\xee");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 0x7fff0000aaaaeeeeU);
	}
	{
		auto data = makeTestString("\x03");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 3U);
	}
	{
		auto data = makeTestString("\xd2\x23\x8a");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 0x12238aU);
	}
	{
		auto data = makeTestString("\xe0\x7f\x23\x8a");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 0x7f238aU);
	}
}

TEST(TestProtocolUnsignedVint, decodeError) {
	{
		cql::ProtocolUnsignedVint value(0);
		std::string data("");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolUnsignedVint value(0);
		auto data = makeTestString("\xe0\x7f\x23");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

