#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolShort.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolShort, getset) {
	cql::ProtocolShort value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(12321);
	ASSERT_EQ(value.get(), 12321);

	value = cql::ProtocolShort(32100);
	ASSERT_EQ(value.get(), 32100);
}

TEST(TestProtocolShort, encode) {
	cql::ProtocolShort value(0x1234);
	std::string data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x12\x34"));
}

TEST(TestProtocolShort, decode) {
	cql::ProtocolShort value(0);
	auto data = makeTestString("\x12\x34");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	value.decode(ptr, end);
	ASSERT_TRUE(ptr == end);
	ASSERT_EQ(value.get(), 0x1234);
}

TEST(TestProtocolShort, decodeError) {
	cql::ProtocolShort value(0);
	auto data = makeTestString("\x12");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
}

