#include <LowLevel/ProtocolTypes/ProtocolResultColumn.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolResultColumn, getset) {
	{
		cql::ProtocolResultColumn value;
		value.setKeySpace("abc");
		value.setTable("asdfg");
		value.setName("q");
		value.setType(cql::ProtocolColumnOption(cql::ColumnType::Int));
		ASSERT_EQ(value.getKeySpace(), "abc");
		ASSERT_EQ(value.getTable(), "asdfg");
		ASSERT_EQ(value.getName(), "q");
		ASSERT_EQ(value.getType().get(), cql::ColumnType::Int);
	}
	{
		cql::ProtocolResultColumn value;
		value.setName("w");
		value.setType(cql::ProtocolColumnOption(cql::ColumnType::Ascii));
		ASSERT_EQ(value.getName(), "w");
		ASSERT_EQ(value.getType().get(), cql::ColumnType::Ascii);
	}
}

TEST(TestProtocolResultColumn, encode) {
	{
		cql::ProtocolResultColumn value;
		value.setKeySpace("abc");
		value.setTable("asdfg");
		value.setName("q");
		value.setType(cql::ProtocolColumnOption(cql::ColumnType::Int));
		std::string data;
		value.encode(data, true);
		ASSERT_EQ(data, makeTestString(
			"\x00\x03""abc"
			"\x00\x05""asdfg"
			"\x00\x01""q"
			"\x00\x09"));
	}
	{
		cql::ProtocolResultColumn value;
		value.setName("w");
		value.setType(cql::ProtocolColumnOption(cql::ColumnType::Ascii));
		std::string data;
		value.encode(data, false);
		ASSERT_EQ(data, makeTestString(
			"\x00\x01""w"
			"\x00\x01"));
	}
}

TEST(TestProtocolResultColumn, decode) {
	cql::ProtocolResultColumn value;
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
		ASSERT_EQ(value.getType().get(), cql::ColumnType::Int);
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
		ASSERT_EQ(value.getType().get(), cql::ColumnType::Ascii);
	}
}

