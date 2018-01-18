#include <LowLevel/ProtocolTypes/CqlProtocolResultRowsMetadata.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolResultRowsMetadata, getset) {
	{
		cql::CqlProtocolResultRowsMetadata value;
		value.setColumnsCount(123);
		value.setPagingState("abc");
		ASSERT_EQ(value.getFlags(),
			cql::CqlResultRowsMetadataFlags::NoMetadata |
			cql::CqlResultRowsMetadataFlags::HasMorePages);
		ASSERT_EQ(value.getColumnsCount(), 123);
		ASSERT_EQ(value.getPagingState(), "abc");
	}
	{
		cql::CqlProtocolResultRowsMetadata value;
		value.setGlobalKeySpaceAndTable("asd", "qwe");
		value.setColumns({ cql::CqlProtocolResultColumn(), cql::CqlProtocolResultColumn() });
		value.getColumns().at(0).setName("a");
		value.getColumns().at(0).setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Int));
		value.getColumns().at(1).setName("b");
		value.getColumns().at(1).setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Ascii));
		ASSERT_EQ(value.getFlags(), cql::CqlResultRowsMetadataFlags::GlobalTableSpec);
		ASSERT_EQ(value.getColumnsCount(), 2);
		ASSERT_EQ(value.getGlobalKeySpace(), "asd");
		ASSERT_EQ(value.getGlobalTable(), "qwe");
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::CqlColumnType::Int);
		ASSERT_EQ(value.getColumns().at(1).getName(), "b");
		ASSERT_EQ(value.getColumns().at(1).getType().get(), cql::CqlColumnType::Ascii);
	}
}

TEST(TestCqlProtocolResultRowsMetadata, encode) {
	{
		cql::CqlProtocolResultRowsMetadata value;
		value.setColumnsCount(123);
		value.setPagingState("abc");
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x06"
			"\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc"));
	}
	{
		cql::CqlProtocolResultRowsMetadata value;
		value.setGlobalKeySpaceAndTable("asd", "qwe");
		value.setColumns({ cql::CqlProtocolResultColumn(), cql::CqlProtocolResultColumn() });
		value.getColumns().at(0).setName("a");
		value.getColumns().at(0).setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Int));
		value.getColumns().at(1).setName("b");
		value.getColumns().at(1).setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Ascii));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x01"
			"\x00\x00\x00\x02"
			"\x00\x03""asd"
			"\x00\x03""qwe"
			"\x00\x01""a""\x00\x09"
			"\x00\x01""b""\x00\x01"));
	}
	{
		cql::CqlProtocolResultRowsMetadata value;
		value.setColumns({ cql::CqlProtocolResultColumn() });
		value.getColumns().at(0).setKeySpace("k");
		value.getColumns().at(0).setTable("t");
		value.getColumns().at(0).setName("a");
		value.getColumns().at(0).setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Int));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x00"
			"\x00\x00\x00\x01"
			"\x00\x01""k""\x00\x01""t""\x00\x01""a""\x00\x09"));
	}
}

TEST(TestCqlProtocolResultRowsMetadata, decode) {
	cql::CqlProtocolResultRowsMetadata value;
	{
		auto data = makeTestString(
			"\x00\x00\x00\x06"
			"\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getFlags(),
			cql::CqlResultRowsMetadataFlags::NoMetadata |
			cql::CqlResultRowsMetadataFlags::HasMorePages);
		ASSERT_EQ(value.getColumnsCount(), 123);
		ASSERT_EQ(value.getPagingState(), "abc");
	}
	{
		auto data = makeTestString(
			"\x00\x00\x00\x01"
			"\x00\x00\x00\x02"
			"\x00\x03""asd"
			"\x00\x03""qwe"
			"\x00\x01""a""\x00\x09"
			"\x00\x01""b""\x00\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getFlags(), cql::CqlResultRowsMetadataFlags::GlobalTableSpec);
		ASSERT_EQ(value.getColumnsCount(), 2);
		ASSERT_EQ(value.getGlobalKeySpace(), "asd");
		ASSERT_EQ(value.getGlobalTable(), "qwe");
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::CqlColumnType::Int);
		ASSERT_EQ(value.getColumns().at(1).getName(), "b");
		ASSERT_EQ(value.getColumns().at(1).getType().get(), cql::CqlColumnType::Ascii);
	}
	{
		auto data = makeTestString(
			"\x00\x00\x00\x00"
			"\x00\x00\x00\x01"
			"\x00\x01""k""\x00\x01""t""\x00\x01""a""\x00\x09");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getFlags(), cql::CqlResultRowsMetadataFlags::None);
		ASSERT_EQ(value.getColumnsCount(), 1);
		ASSERT_EQ(value.getColumns().at(0).getKeySpace(), "k");
		ASSERT_EQ(value.getColumns().at(0).getTable(), "t");
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::CqlColumnType::Int);
	}
}

