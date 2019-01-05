#include <LowLevel/ProtocolTypes/ProtocolResultRowsMetadata.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolResultRowsMetadata, getset) {
	{
		cql::ProtocolResultRowsMetadata value;
		value.setColumnsCount(123);
		value.setPagingState("abc");
		ASSERT_EQ(value.getFlags(),
			cql::ResultRowsMetadataFlags::NoMetadata |
			cql::ResultRowsMetadataFlags::HasMorePages);
		ASSERT_EQ(value.getColumnsCount(), 123U);
		ASSERT_EQ(value.getPagingState(), "abc");
	}
	{
		cql::ProtocolResultRowsMetadata value;
		value.setGlobalKeySpaceAndTable("asd", "qwe");
		value.setColumns({ cql::ProtocolResultColumn(), cql::ProtocolResultColumn() });
		value.getColumns().at(0).setName("a");
		value.getColumns().at(0).setType(cql::ProtocolColumnOption(cql::ColumnType::Int));
		value.getColumns().at(1).setName("b");
		value.getColumns().at(1).setType(cql::ProtocolColumnOption(cql::ColumnType::Ascii));
		ASSERT_EQ(value.getFlags(), cql::ResultRowsMetadataFlags::GlobalTableSpec);
		ASSERT_EQ(value.getColumnsCount(), 2U);
		ASSERT_EQ(value.getGlobalKeySpace(), "asd");
		ASSERT_EQ(value.getGlobalTable(), "qwe");
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::ColumnType::Int);
		ASSERT_EQ(value.getColumns().at(1).getName(), "b");
		ASSERT_EQ(value.getColumns().at(1).getType().get(), cql::ColumnType::Ascii);
	}
}

TEST(TestProtocolResultRowsMetadata, encode) {
	{
		cql::ProtocolResultRowsMetadata value;
		value.setColumnsCount(123);
		value.setPagingState("abc");
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x06"
			"\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc"));
	}
	{
		cql::ProtocolResultRowsMetadata value;
		value.setGlobalKeySpaceAndTable("asd", "qwe");
		value.setColumns({ cql::ProtocolResultColumn(), cql::ProtocolResultColumn() });
		value.getColumns().at(0).setName("a");
		value.getColumns().at(0).setType(cql::ProtocolColumnOption(cql::ColumnType::Int));
		value.getColumns().at(1).setName("b");
		value.getColumns().at(1).setType(cql::ProtocolColumnOption(cql::ColumnType::Ascii));
		std::string data;
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
		cql::ProtocolResultRowsMetadata value;
		value.setColumns({ cql::ProtocolResultColumn() });
		value.getColumns().at(0).setKeySpace("k");
		value.getColumns().at(0).setTable("t");
		value.getColumns().at(0).setName("a");
		value.getColumns().at(0).setType(cql::ProtocolColumnOption(cql::ColumnType::Int));
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x00"
			"\x00\x00\x00\x01"
			"\x00\x01""k""\x00\x01""t""\x00\x01""a""\x00\x09"));
	}
}

TEST(TestProtocolResultRowsMetadata, decode) {
	cql::ProtocolResultRowsMetadata value;
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
			cql::ResultRowsMetadataFlags::NoMetadata |
			cql::ResultRowsMetadataFlags::HasMorePages);
		ASSERT_EQ(value.getColumnsCount(), 123U);
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
		ASSERT_EQ(value.getFlags(), cql::ResultRowsMetadataFlags::GlobalTableSpec);
		ASSERT_EQ(value.getColumnsCount(), 2U);
		ASSERT_EQ(value.getGlobalKeySpace(), "asd");
		ASSERT_EQ(value.getGlobalTable(), "qwe");
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::ColumnType::Int);
		ASSERT_EQ(value.getColumns().at(1).getName(), "b");
		ASSERT_EQ(value.getColumns().at(1).getType().get(), cql::ColumnType::Ascii);
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
		ASSERT_EQ(value.getFlags(), cql::ResultRowsMetadataFlags::None);
		ASSERT_EQ(value.getColumnsCount(), 1U);
		ASSERT_EQ(value.getColumns().at(0).getKeySpace(), "k");
		ASSERT_EQ(value.getColumns().at(0).getTable(), "t");
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::ColumnType::Int);
	}
}

