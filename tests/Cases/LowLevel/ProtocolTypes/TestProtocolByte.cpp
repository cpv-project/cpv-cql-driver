#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolByte.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolByte, getset) {
	cql::ProtocolByte value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(123);
	ASSERT_EQ(value.get(), 123);

	value = cql::ProtocolByte(255);
	ASSERT_EQ(value.get(), 255);
}

TEST(TestProtocolByte, encode) {
	cql::ProtocolByte value(123);
	std::string data;
	value.encode(data);
	ASSERT_EQ(data, std::string("\x7b"));
}

TEST(TestProtocolByte, decode) {
	cql::ProtocolByte value(0);
	std::string data("\x7b");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	value.decode(ptr, end);
	ASSERT_TRUE(ptr == end);
	ASSERT_EQ(value.get(), 123);
}

TEST(TestProtocolByte, decodeError) {
	cql::ProtocolByte value(0);
	std::string data;
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
}

