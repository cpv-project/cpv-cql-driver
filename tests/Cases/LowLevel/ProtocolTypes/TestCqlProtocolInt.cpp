#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolInt, getset) {
	cql::CqlProtocolInt value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff0000);
	ASSERT_EQ(value.get(), 0x7fff0000);

	value = cql::CqlProtocolInt(-0x80000000);
	ASSERT_EQ(value.get(), -0x80000000);
}

TEST(TestCqlProtocolInt, encode) {
	cql::CqlProtocolInt value(0x12345678);
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, seastar::sstring("\x12\x34\x56\x78"));
}

TEST(TestCqlProtocolInt, decode) {
	cql::CqlProtocolInt value(0);
	seastar::sstring data("\x12\x34\x56\x78");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	value.decode(ptr, end);
	ASSERT_TRUE(ptr == end);
	ASSERT_EQ(value.get(), 0x12345678);
}

TEST(TestCqlProtocolInt, decodeError) {
	cql::CqlProtocolInt value(0);
	seastar::sstring data("\x12");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
}

