#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolInet.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolInet, getset) {
	cql::ProtocolInet value({ seastar::net::inet_address("127.0.0.1"), 123 });
	ASSERT_EQ(value.get(),
		cql::ProtocolInet::ValueType(seastar::net::inet_address("127.0.0.1"), 123));
	value.set({ seastar::net::inet_address("0:0:FF:D:C:B:A:1"), 321 });
	ASSERT_EQ(value.get(),
		cql::ProtocolInet::ValueType(seastar::net::inet_address("0:0:FF:D:C:B:A:1"), 321));

	value = cql::ProtocolInet({ seastar::net::inet_address("0.0.0.0"), 0 });
	ASSERT_EQ(value.get(),
		cql::ProtocolInet::ValueType(seastar::net::inet_address("0.0.0.0"), 0));
}

TEST(TestProtocolInet, encode) {
	{
		cql::ProtocolInet value({ seastar::net::inet_address("127.0.0.1"), 0x89aa });
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x04\x7f\x00\x00\x01\x00\x00\x89\xaa"));
	}
	{
		cql::ProtocolInet value({ seastar::net::inet_address("0:0:FF:D:C:B:A:1"), 0xaa89 });
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x10\x00\x00\x00\x00\x00\xff\x00\x0d\x00\x0c\x00\x0b\x00\x0a\x00\x01\x00\x00\xaa\x89"));
	}
}

TEST(TestProtocolInet, decode) {
	cql::ProtocolInet value;
	{
		auto data = makeTestString("\x04\x7f\x00\x00\x01\x00\x00\x89\xaa");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(),
			cql::ProtocolInet::ValueType(seastar::net::inet_address("127.0.0.1"), 0x89aa));
	}
	{
		auto data = makeTestString(
			"\x10\x00\x00\x00\x00\x00\xff\x00\x0d\x00\x0c\x00\x0b\x00\x0a\x00\x01\x00\x00\xaa\x89");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(),
			cql::ProtocolInet::ValueType(seastar::net::inet_address("0:0:FF:D:C:B:A:1"), 0xaa89));
	}
}

TEST(TestProtocolInet, decodeError) {
	{
		cql::ProtocolInet value;
		auto data = makeTestString("\x04\x7f\x00\x00\x01\x00\x00\x89");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolInet value;
		auto data = makeTestString("\x04\x7f\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolInet value;
		auto data = makeTestString("\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolInet value;
		std::string data;
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

