#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <CQLDriver/Common/Exceptions/FormatException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolUUID.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolUUID, getset) {
	cql::ProtocolUUID uuid("00112233-4455-6677-8899-aabbccddeeff");
	ASSERT_EQ(uuid.get().first, 0x0011223344556677);
	ASSERT_EQ(uuid.get().second, 0x8899aabbccddeeff);
	ASSERT_EQ(uuid.str(), "00112233-4455-6677-8899-AABBCCDDEEFF");

	uuid.set(std::make_pair(0x12345678abcdefaaU, 0x87654321ffabcdefU));
	ASSERT_EQ(uuid.get().first, 0x12345678abcdefaa);
	ASSERT_EQ(uuid.get().second, 0x87654321ffabcdef);
	ASSERT_EQ(uuid.str(), "12345678-ABCD-EFAA-8765-4321FFABCDEF");

	uuid = cql::ProtocolUUID(std::make_pair(0x0011223344556677U, 0x8899aabbccddeeffU));
	ASSERT_EQ(uuid.get().first, 0x0011223344556677);
	ASSERT_EQ(uuid.get().second, 0x8899aabbccddeeff);
	ASSERT_EQ(uuid.str(), "00112233-4455-6677-8899-AABBCCDDEEFF");
}

TEST(TestProtocolUUID, encode) {
	{
		cql::ProtocolUUID value("00112233-4455-6677-8899-aabbccddeeff");
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff"));
	}
	{
		cql::ProtocolUUID value("12345678-ABCD-EFAA-8765-4321FFABCDEF");
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x12\x34\x56\x78\xab\xcd\xef\xaa\x87\x65\x43\x21\xff\xab\xcd\xef"));
	}
}

TEST(TestProtocolUUID, decode) {
	cql::ProtocolUUID value;
	{
		auto data = makeTestString(
			"\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.str(), "00112233-4455-6677-8899-AABBCCDDEEFF");
	}
	{
		auto data = makeTestString(
			"\x12\x34\x56\x78\xab\xcd\xef\xaa\x87\x65\x43\x21\xff\xab\xcd\xef");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.str(), "12345678-ABCD-EFAA-8765-4321FFABCDEF");
	}
}

