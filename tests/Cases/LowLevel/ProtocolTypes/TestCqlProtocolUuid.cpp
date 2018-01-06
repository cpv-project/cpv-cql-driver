#include <CqlDriver/Exceptions/CqlInternalException.hpp>
#include <CqlDriver/Exceptions/CqlFormatException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolUuid.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolUuid, getset) {
	cql::CqlProtocolUuid uuid("00112233-4455-6677-8899-aabbccddeeff");
	ASSERT_EQ(uuid.get().first, 0x0011223344556677);
	ASSERT_EQ(uuid.get().second, 0x8899aabbccddeeff);
	ASSERT_EQ(uuid.str(), "00112233-4455-6677-8899-AABBCCDDEEFF");

	uuid.set(std::make_pair(0x12345678abcdefaaU, 0x87654321ffabcdefU));
	ASSERT_EQ(uuid.get().first, 0x12345678abcdefaa);
	ASSERT_EQ(uuid.get().second, 0x87654321ffabcdef);
	ASSERT_EQ(uuid.str(), "12345678-ABCD-EFAA-8765-4321FFABCDEF");

	uuid = cql::CqlProtocolUuid(0xabcdabcd85868788, 0x05060708cdefcdef);
	ASSERT_EQ(uuid.get().first, 0xabcdabcd85868788);
	ASSERT_EQ(uuid.get().second, 0x05060708cdefcdef);
	ASSERT_EQ(uuid.str(), "ABCDABCD-8586-8788-0506-0708CDEFCDEF");

	uuid = cql::CqlProtocolUuid(std::make_pair(0x0011223344556677U, 0x8899aabbccddeeffU));
	ASSERT_EQ(uuid.get().first, 0x0011223344556677);
	ASSERT_EQ(uuid.get().second, 0x8899aabbccddeeff);
	ASSERT_EQ(uuid.str(), "00112233-4455-6677-8899-AABBCCDDEEFF");
}

TEST(TestCqlProtocolUuid, encode) {
	{
		cql::CqlProtocolUuid value("00112233-4455-6677-8899-aabbccddeeff");
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring(
			"\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff", 16));
	}
	{
		cql::CqlProtocolUuid value("12345678-ABCD-EFAA-8765-4321FFABCDEF");
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring(
			"\x12\x34\x56\x78\xab\xcd\xef\xaa\x87\x65\x43\x21\xff\xab\xcd\xef", 16));
	}
}

TEST(TestCqlProtocolUuid, decode) {
	{
		cql::CqlProtocolUuid value(0, 0);
		seastar::sstring data(
			"\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff", 16);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.str(), "00112233-4455-6677-8899-AABBCCDDEEFF");
	}
	{
		cql::CqlProtocolUuid value(0, 0);
		seastar::sstring data(
			"\x12\x34\x56\x78\xab\xcd\xef\xaa\x87\x65\x43\x21\xff\xab\xcd\xef", 16);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.str(), "12345678-ABCD-EFAA-8765-4321FFABCDEF");
	}
}

TEST(TestCqlProtocolUuid, getEmpty) {
	auto& empty = cql::CqlProtocolUuid::getEmpty();
	ASSERT_EQ(empty.str(), "00000000-0000-0000-0000-000000000000");
	ASSERT_EQ(empty.get(), cql::CqlProtocolUuid::getEmpty().get());
}

TEST(TestCqlProtocolUuid, makeRandom) {
	auto a = cql::CqlProtocolUuid::makeRandom();
	auto b = cql::CqlProtocolUuid::makeRandom();
	ASSERT_FALSE(a.get() == b.get());
	ASSERT_EQ(a.get().first & 0xf000, 0x4000); // version
	ASSERT_EQ(a.get().second & 0xc000'0000'0000'0000, 0x8000'0000'0000'0000); // variant
}

