#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolInetAddr.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolInetAddr, getset) {
	cql::CqlProtocolInetAddr value(seastar::net::inet_address("127.0.0.1"));
	ASSERT_EQ(value.get(), seastar::net::inet_address("127.0.0.1"));
	value.set(seastar::net::inet_address("0:0:FF:D:C:B:A:1"));
	ASSERT_EQ(value.get(), seastar::net::inet_address("0:0:FF:D:C:B:A:1"));

	value = cql::CqlProtocolInetAddr(seastar::net::inet_address("0.0.0.0"));
	ASSERT_EQ(value.get(), seastar::net::inet_address("0.0.0.0"));
}

TEST(TestCqlProtocolInetAddr, encode) {
	{
		cql::CqlProtocolInetAddr value(seastar::net::inet_address("127.0.0.1"));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\x04\x7f\x00\x00\x01", 5));
	}
	{
		cql::CqlProtocolInetAddr value(seastar::net::inet_address("0:0:FF:D:C:B:A:1"));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring(
			"\x10\x00\x00\x00\x00\x00\xff\x00\x0d\x00\x0c\x00\x0b\x00\x0a\x00\x01", 17));
	}
}

TEST(TestCqlProtocolInetAddr, decode) {
	cql::CqlProtocolInetAddr value;
	{
		seastar::sstring data("\x04\x7f\x00\x00\x01", 5);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), seastar::net::inet_address("127.0.0.1"));
	}
	{
		seastar::sstring data(
			"\x10\x00\x00\x00\x00\x00\xff\x00\x0d\x00\x0c\x00\x0b\x00\x0a\x00\x01", 17);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), seastar::net::inet_address("0:0:FF:D:C:B:A:1"));
	}
}

TEST(TestCqlProtocolInetAddr, decodeError) {
	{
		cql::CqlProtocolInetAddr value;
		seastar::sstring data("\x04\x7f\x00", 3);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolInetAddr value;
		seastar::sstring data("\x00", 1);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolInetAddr value;
		seastar::sstring data;
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

