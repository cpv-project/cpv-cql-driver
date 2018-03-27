#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolInetAddr.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolInetAddr, getset) {
	cql::ProtocolInetAddr value(seastar::net::inet_address("127.0.0.1"));
	ASSERT_EQ(value.get(), seastar::net::inet_address("127.0.0.1"));
	value.set(seastar::net::inet_address("0:0:FF:D:C:B:A:1"));
	ASSERT_EQ(value.get(), seastar::net::inet_address("0:0:FF:D:C:B:A:1"));

	value = cql::ProtocolInetAddr(seastar::net::inet_address("0.0.0.0"));
	ASSERT_EQ(value.get(), seastar::net::inet_address("0.0.0.0"));
}

TEST(TestProtocolInetAddr, encode) {
	{
		cql::ProtocolInetAddr value(seastar::net::inet_address("127.0.0.1"));
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x04\x7f\x00\x00\x01"));
	}
	{
		cql::ProtocolInetAddr value(seastar::net::inet_address("0:0:FF:D:C:B:A:1"));
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x10\x00\x00\x00\x00\x00\xff\x00\x0d\x00\x0c\x00\x0b\x00\x0a\x00\x01"));
	}
}

TEST(TestProtocolInetAddr, decode) {
	cql::ProtocolInetAddr value;
	{
		auto data = makeTestString("\x04\x7f\x00\x00\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), seastar::net::inet_address("127.0.0.1"));
	}
	{
		auto data = makeTestString(
			"\x10\x00\x00\x00\x00\x00\xff\x00\x0d\x00\x0c\x00\x0b\x00\x0a\x00\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), seastar::net::inet_address("0:0:FF:D:C:B:A:1"));
	}
}

TEST(TestProtocolInetAddr, decodeError) {
	{
		cql::ProtocolInetAddr value;
		auto data = makeTestString("\x04\x7f\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolInetAddr value;
		auto data = makeTestString("\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolInetAddr value;
		std::string data("");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

