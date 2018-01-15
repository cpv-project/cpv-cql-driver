#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolLong.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolLong, getset) {
	cql::CqlProtocolLong value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff0000aaaaeeee);
	ASSERT_EQ(value.get(), 0x7fff0000aaaaeeee);

	value = cql::CqlProtocolLong(-3);
	ASSERT_EQ(value.get(), -3);
}

TEST(TestCqlProtocolLong, encode) {
	{
		cql::CqlProtocolLong value(0x7fff0000aaaaeeee);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x7f\xff\x00\x00\xaa\xaa\xee\xee"));
	}
	{
		cql::CqlProtocolLong value(-3);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\xff\xff\xff\xff\xff\xff\xff\xfd"));
	}
}

TEST(TestCqlProtocolLong, decode) {
	cql::CqlProtocolLong value(0);
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

TEST(TestCqlProtocolLong, decodeError) {
	cql::CqlProtocolLong value(0);
	seastar::sstring data("\x12");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
}

