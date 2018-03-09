#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolLong.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolLong, getset) {
	cql::ProtocolLong value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff0000aaaaeeee);
	ASSERT_EQ(value.get(), 0x7fff0000aaaaeeee);

	value = cql::ProtocolLong(-3);
	ASSERT_EQ(value.get(), -3);
}

TEST(TestProtocolLong, encode) {
	{
		cql::ProtocolLong value(0x7fff0000aaaaeeee);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x7f\xff\x00\x00\xaa\xaa\xee\xee"));
	}
	{
		cql::ProtocolLong value(-3);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\xff\xff\xff\xff\xff\xff\xff\xfd"));
	}
}

TEST(TestProtocolLong, decode) {
	cql::ProtocolLong value(0);
	{
		auto data = makeTestString("\x7f\xff\x00\x00\xaa\xaa\xee\xee");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), 0x7fff0000aaaaeeee);
	}
	{
		auto data = makeTestString("\xff\xff\xff\xff\xff\xff\xff\xfd");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), -3);
	}
}

TEST(TestProtocolLong, decodeError) {
	cql::ProtocolLong value(0);
	seastar::sstring data("\x12");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
}

