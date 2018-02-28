#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlInet.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlInet, getset) {
	cql::CqlInet value;
	ASSERT_EQ(value, seastar::net::inet_address("0.0.0.0"));

	value.set("127.0.0.1");
	ASSERT_EQ(value.get(), seastar::net::inet_address("127.0.0.1"));

	value.set(seastar::net::inet_address("0:0:FF:D:C:B:A:1"));
	ASSERT_EQ(value.get(), seastar::net::inet_address("0:0:FF:D:C:B:A:1"));

	value = cql::CqlInet("0.0.0.0");
	ASSERT_EQ(value.get(), seastar::net::inet_address("0.0.0.0"));
}

TEST(TestCqlInet, encode) {
	{
		cql::CqlInet value("127.0.0.1");
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x7f\x00\x00\x01"));
	}
	{
		cql::CqlInet value("0:0:FF:D:C:B:A:1");
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x00\x00\xff\x00\x0d\x00\x0c\x00\x0b\x00\x0a\x00\x01"));
	}}

TEST(TestCqlInet, decode) {
	{
		cql::CqlInet value;
		auto data = makeTestString("\x7f\x00\x00\x01");
		value.decode(data.data(), data.size());
		ASSERT_EQ(value, seastar::net::inet_address("127.0.0.1"));
	}
	{
		cql::CqlInet value;
		auto data = makeTestString(
			"\x00\x00\x00\x00\x00\xff\x00\x0d\x00\x0c\x00\x0b\x00\x0a\x00\x01");
		value.decode(data.data(), data.size());
		ASSERT_EQ(value, seastar::net::inet_address("0:0:FF:D:C:B:A:1"));
	}
	{
		cql::CqlInet value("127.0.0.1");
		auto data = makeTestString("");
		value.decode(data.data(), data.size());
		ASSERT_EQ(value, seastar::net::inet_address("0.0.0.0"));
	}
}

TEST(TestCqlInet, decodeError) {
	cql::CqlInet value;
	auto data = makeTestString("\x7f\x00\x01");
	ASSERT_THROWS(cql::CqlDecodeException, value.decode(data.data(), data.size()));
}

TEST(TestCqlInet, operations) {
	{
		// assign and cast
		cql::CqlInet value;
		value = "127.0.0.1";
		cql::CqlInet::CqlUnderlyingType ipAddress = value;
		ASSERT_EQ(ipAddress, seastar::net::inet_address("127.0.0.1"));
	}
	{
		// dereference
		cql::CqlInet value("127.0.0.1");
		ASSERT_EQ(*value, seastar::net::inet_address("127.0.0.1"));
	}
	{
		// get pointer
		cql::CqlInet value("127.0.0.1");
		ASSERT_EQ(
			seastar::sstring(reinterpret_cast<const char*>(value->data()), value->size()),
			makeTestString("\x7f\x00\x00\x01"));
	}
	{
		// equal to
		ASSERT_TRUE(cql::CqlInet("127.0.0.1") == cql::CqlInet("127.0.0.1"));
		ASSERT_TRUE(cql::CqlInet("127.0.0.1") == seastar::net::inet_address("127.0.0.1"));
		ASSERT_TRUE(seastar::net::inet_address("127.0.0.1") == cql::CqlInet("127.0.0.1"));
		ASSERT_FALSE(cql::CqlInet("127.0.0.1") == cql::CqlInet("127.0.0.2"));
		ASSERT_FALSE(cql::CqlInet("127.0.0.1") == seastar::net::inet_address("127.0.0.2"));
		ASSERT_FALSE(seastar::net::inet_address("127.0.0.1") == cql::CqlInet("127.0.0.2"));
	}
	{
		// not equal to
		ASSERT_FALSE(cql::CqlInet("127.0.0.1") != cql::CqlInet("127.0.0.1"));
		ASSERT_FALSE(cql::CqlInet("127.0.0.1") != seastar::net::inet_address("127.0.0.1"));
		ASSERT_FALSE(seastar::net::inet_address("127.0.0.1") != cql::CqlInet("127.0.0.1"));
		ASSERT_TRUE(cql::CqlInet("127.0.0.1") != cql::CqlInet("127.0.0.2"));
		ASSERT_TRUE(cql::CqlInet("127.0.0.1") != seastar::net::inet_address("127.0.0.2"));
		ASSERT_TRUE(seastar::net::inet_address("127.0.0.1") != cql::CqlInet("127.0.0.2"));
	}
}

