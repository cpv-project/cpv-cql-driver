#include <LowLevel/ProtocolTypes/CqlProtocolPrepareParameters.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolPrepareParameters, getset) {
	cql::CqlProtocolPrepareParameters value;
	value.setKeySpace("abc");
	ASSERT_EQ(value.getFlags(), cql::CqlPrepareParametersFlags::WithKeySpace);
	ASSERT_EQ(value.getKeySpace(), "abc");
}

TEST(TestCqlProtocolPrepareParameters, encode) {
	{
		cql::CqlProtocolPrepareParameters value;
		value.setKeySpace("abc");
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x01"
			"\x00\x03""abc"));
	}
	{
		cql::CqlProtocolPrepareParameters value;
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));
	}
}

TEST(TestCqlProtocolPrepareParameters, decode) {
	cql::CqlProtocolPrepareParameters value;
	{
		auto data = makeTestString(
			"\x00\x00\x00\x01"
			"\x00\x03""abc");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getFlags(), cql::CqlPrepareParametersFlags::WithKeySpace);
		ASSERT_EQ(value.getKeySpace(), "abc");
	}
	{
		auto data = makeTestString("\x00\x00\x00\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getFlags(), cql::CqlPrepareParametersFlags::None);
	}
}

