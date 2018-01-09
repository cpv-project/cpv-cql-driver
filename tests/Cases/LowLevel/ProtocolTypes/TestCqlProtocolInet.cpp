#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolInet.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolInet, getset) {
	cql::CqlProtocolInet value({ seastar::net::inet_address("127.0.0.1"), 123 });
	ASSERT_EQ(value.get(),
		cql::CqlProtocolInet::ValueType(seastar::net::inet_address("127.0.0.1"), 123));
	value.set({ seastar::net::inet_address("0:0:FF:D:C:B:A:1"), 321 });
	ASSERT_EQ(value.get(),
		cql::CqlProtocolInet::ValueType(seastar::net::inet_address("0:0:FF:D:C:B:A:1"), 321));

	value = cql::CqlProtocolInet({ seastar::net::inet_address("0.0.0.0"), 0 });
	ASSERT_EQ(value.get(),
		cql::CqlProtocolInet::ValueType(seastar::net::inet_address("0.0.0.0"), 0));
}

TEST(TestCqlProtocolInet, encode) {
	{
		cql::CqlProtocolInet value({ seastar::net::inet_address("127.0.0.1"), 0x89aa });
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\x04\x7f\x00\x00\x01\x00\x00\x89\xaa", 9));
	}
	{
		cql::CqlProtocolInet value({ seastar::net::inet_address("0:0:FF:D:C:B:A:1"), 0xaa89 });
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring(
			"\x10\x00\x00\x00\x00\x00\xff\x00\x0d\x00\x0c\x00\x0b\x00\x0a\x00\x01\x00\x00\xaa\x89", 21));
	}
}

TEST(TestCqlProtocolInet, decode) {
	cql::CqlProtocolInet value;
	{
		seastar::sstring data("\x04\x7f\x00\x00\x01\x00\x00\x89\xaa", 9);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(),
			cql::CqlProtocolInet::ValueType(seastar::net::inet_address("127.0.0.1"), 0x89aa));
	}
	{
		seastar::sstring data(
			"\x10\x00\x00\x00\x00\x00\xff\x00\x0d\x00\x0c\x00\x0b\x00\x0a\x00\x01\x00\x00\xaa\x89", 21);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(),
			cql::CqlProtocolInet::ValueType(seastar::net::inet_address("0:0:FF:D:C:B:A:1"), 0xaa89));
	}
}

TEST(TestCqlProtocolInet, decodeError) {
	{
		cql::CqlProtocolInet value;
		seastar::sstring data("\x04\x7f\x00\x00\x01\x00\x00\x89", 8);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolInet value;
		seastar::sstring data("\x04\x7f\x00", 3);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolInet value;
		seastar::sstring data("\x00", 1);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolInet value;
		seastar::sstring data;
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

