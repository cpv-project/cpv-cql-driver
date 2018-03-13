#include <CQLDriver/Common/ColumnTypes/Inet.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestInet, getset) {
	cql::Inet value;
	ASSERT_EQ(value, seastar::net::inet_address("0.0.0.0"));

	value.set("127.0.0.1");
	ASSERT_EQ(value.get(), seastar::net::inet_address("127.0.0.1"));

	value.set(seastar::net::inet_address("0:0:FF:D:C:B:A:1"));
	ASSERT_EQ(value.get(), seastar::net::inet_address("0:0:FF:D:C:B:A:1"));

	value = cql::Inet("0.0.0.0");
	ASSERT_EQ(value.get(), seastar::net::inet_address("0.0.0.0"));
}

TEST(TestInet, encodeBody) {
	{
		cql::Inet value("127.0.0.1");
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x7f\x00\x00\x01"));
	}
	{
		cql::Inet value("0:0:FF:D:C:B:A:1");
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x00\x00\xff\x00\x0d\x00\x0c\x00\x0b\x00\x0a\x00\x01"));
	}}

TEST(TestInet, decodeBody) {
	{
		cql::Inet value;
		auto data = makeTestString("\x7f\x00\x00\x01");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value, seastar::net::inet_address("127.0.0.1"));
	}
	{
		cql::Inet value;
		auto data = makeTestString(
			"\x00\x00\x00\x00\x00\xff\x00\x0d\x00\x0c\x00\x0b\x00\x0a\x00\x01");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value, seastar::net::inet_address("0:0:FF:D:C:B:A:1"));
	}
	{
		cql::Inet value("127.0.0.1");
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value, seastar::net::inet_address("0.0.0.0"));
	}
}

TEST(TestInet, decodeBodyError) {
	cql::Inet value;
	auto data = makeTestString("\x7f\x00\x01");
	ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
}

TEST(TestInet, operations) {
	{
		// assign and cast
		cql::Inet value;
		value = "127.0.0.1";
		cql::Inet::UnderlyingType ipAddress = value;
		ASSERT_EQ(ipAddress, seastar::net::inet_address("127.0.0.1"));
	}
	{
		// dereference
		cql::Inet value("127.0.0.1");
		ASSERT_EQ(*value, seastar::net::inet_address("127.0.0.1"));
	}
	{
		// get pointer
		cql::Inet value("127.0.0.1");
		ASSERT_EQ(
			std::string(reinterpret_cast<const char*>(value->data()), value->size()),
			makeTestString("\x7f\x00\x00\x01"));
	}
	{
		// equal to
		ASSERT_TRUE(cql::Inet("127.0.0.1") == cql::Inet("127.0.0.1"));
		ASSERT_TRUE(cql::Inet("127.0.0.1") == seastar::net::inet_address("127.0.0.1"));
		ASSERT_TRUE(seastar::net::inet_address("127.0.0.1") == cql::Inet("127.0.0.1"));
		ASSERT_FALSE(cql::Inet("127.0.0.1") == cql::Inet("127.0.0.2"));
		ASSERT_FALSE(cql::Inet("127.0.0.1") == seastar::net::inet_address("127.0.0.2"));
		ASSERT_FALSE(seastar::net::inet_address("127.0.0.1") == cql::Inet("127.0.0.2"));
	}
	{
		// not equal to
		ASSERT_FALSE(cql::Inet("127.0.0.1") != cql::Inet("127.0.0.1"));
		ASSERT_FALSE(cql::Inet("127.0.0.1") != seastar::net::inet_address("127.0.0.1"));
		ASSERT_FALSE(seastar::net::inet_address("127.0.0.1") != cql::Inet("127.0.0.1"));
		ASSERT_TRUE(cql::Inet("127.0.0.1") != cql::Inet("127.0.0.2"));
		ASSERT_TRUE(cql::Inet("127.0.0.1") != seastar::net::inet_address("127.0.0.2"));
		ASSERT_TRUE(seastar::net::inet_address("127.0.0.1") != cql::Inet("127.0.0.2"));
	}
}

