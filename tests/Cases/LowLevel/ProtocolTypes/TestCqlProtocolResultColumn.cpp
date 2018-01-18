#include <LowLevel/ProtocolTypes/CqlProtocolResultColumn.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolResultColumn, getset) {
	{
		cql::CqlProtocolResultColumn value;
		value.setKeySpace("abc");
		value.setTable("asdfg");
		value.setName("q");
		value.setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Int));
		ASSERT_EQ(value.getKeySpace(), "abc");
		ASSERT_EQ(value.getTable(), "asdfg");
		ASSERT_EQ(value.getName(), "q");
		ASSERT_EQ(value.getType().get(), cql::CqlColumnType::Int);
	}
	{
		cql::CqlProtocolResultColumn value;
		value.setName("w");
		value.setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Ascii));
		ASSERT_EQ(value.getName(), "w");
		ASSERT_EQ(value.getType().get(), cql::CqlColumnType::Ascii);
	}
}

TEST(TestCqlProtocolResultColumn, encode) {
	{
		cql::CqlProtocolResultColumn value;
		value.setKeySpace("abc");
		value.setTable("asdfg");
		value.setName("q");
		value.setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Int));
		seastar::sstring data;
		value.encode(data, true);
		ASSERT_EQ(data, makeTestString(
			"\x00\x03""abc"
			"\x00\x05""asdfg"
			"\x00\x01""q"
			"\x00\x09"));
	}
	{
		cql::CqlProtocolResultColumn value;
		value.setName("w");
		value.setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Ascii));
		seastar::sstring data;
		value.encode(data, false);
		ASSERT_EQ(data, makeTestString(
			"\x00\x01""w"
			"\x00\x01"));
	}
}

TEST(TestCqlProtocolResultColumn, decode) {
	cql::CqlProtocolResultColumn value;
	{
		auto data = makeTestString(
			"\x00\x03""abc"
			"\x00\x05""asdfg"
			"\x00\x01""q"
			"\x00\x09");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end, true);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getKeySpace(), "abc");
		ASSERT_EQ(value.getTable(), "asdfg");
		ASSERT_EQ(value.getName(), "q");
		ASSERT_EQ(value.getType().get(), cql::CqlColumnType::Int);
	}
	{
		auto data = makeTestString(
			"\x00\x01""w"
			"\x00\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end, false);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getName(), "w");
		ASSERT_EQ(value.getType().get(), cql::CqlColumnType::Ascii);
	}
}

