#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolInt, getset) {
	cql::ProtocolInt value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff0000);
	ASSERT_EQ(value.get(), 0x7fff0000);

	value = cql::ProtocolInt(-0x80000000);
	ASSERT_EQ(value.get(), static_cast<std::int32_t>(-0x80000000));
}

TEST(TestProtocolInt, encode) {
	cql::ProtocolInt value(0x12345678);
	std::string data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x12\x34\x56\x78"));
}

TEST(TestProtocolInt, decode) {
	cql::ProtocolInt value(0);
	auto data = makeTestString("\x12\x34\x56\x78");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	value.decode(ptr, end);
	ASSERT_TRUE(ptr == end);
	ASSERT_EQ(value.get(), 0x12345678);
}

TEST(TestProtocolInt, decodeError) {
	cql::ProtocolInt value(0);
	auto data = makeTestString("\x12");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
}

