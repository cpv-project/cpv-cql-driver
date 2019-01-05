#include <LowLevel/ProtocolTypes/ProtocolResultPreparedMetadata.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolResultPreparedMetadata, getset) {
	{
		cql::ProtocolResultPreparedMetadata value;
		value.setPartitionKeyIndexes({ cql::ProtocolShort(1) });
		value.setColumns({ cql::ProtocolResultColumn(), cql::ProtocolResultColumn() });
		value.getColumns().at(0).setKeySpace("k");
		value.getColumns().at(0).setTable("t");
		value.getColumns().at(0).setName("a");
		value.getColumns().at(0).setType(cql::ProtocolColumnOption(cql::ColumnType::Int));
		value.getColumns().at(1).setKeySpace("K");
		value.getColumns().at(1).setTable("T");
		value.getColumns().at(1).setName("b");
		value.getColumns().at(1).setType(cql::ProtocolColumnOption(cql::ColumnType::Ascii));
		ASSERT_EQ(value.getFlags(), cql::ResultPreparedMetadataFlags::None);
		ASSERT_EQ(value.getPartitionKeyIndexes().size(), 1U);
		ASSERT_EQ(value.getPartitionKeyIndexes().at(0).get(), 1);
		ASSERT_EQ(value.getColumns().size(), 2U);
		ASSERT_EQ(value.getColumns().at(0).getKeySpace(), "k");
		ASSERT_EQ(value.getColumns().at(0).getTable(), "t");
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::ColumnType::Int);
		ASSERT_EQ(value.getColumns().at(1).getKeySpace(), "K");
		ASSERT_EQ(value.getColumns().at(1).getTable(), "T");
		ASSERT_EQ(value.getColumns().at(1).getName(), "b");
		ASSERT_EQ(value.getColumns().at(1).getType().get(), cql::ColumnType::Ascii);
	}
	{
		cql::ProtocolResultPreparedMetadata value;
		value.setGlobalKeySpaceAndTable("k0", "t0");
		value.setColumns({ cql::ProtocolResultColumn(), cql::ProtocolResultColumn() });
		value.getColumns().at(0).setName("a");
		value.getColumns().at(0).setType(cql::ProtocolColumnOption(cql::ColumnType::Int));
		value.getColumns().at(1).setName("b");
		value.getColumns().at(1).setType(cql::ProtocolColumnOption(cql::ColumnType::Ascii));
		ASSERT_EQ(value.getFlags(), cql::ResultPreparedMetadataFlags::GlobalTableSpec);
		ASSERT_TRUE(value.getPartitionKeyIndexes().empty());
		ASSERT_EQ(value.getGlobalKeySpace(), "k0");
		ASSERT_EQ(value.getGlobalTable(), "t0");
		ASSERT_EQ(value.getColumns().size(), 2U);
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::ColumnType::Int);
		ASSERT_EQ(value.getColumns().at(1).getName(), "b");
		ASSERT_EQ(value.getColumns().at(1).getType().get(), cql::ColumnType::Ascii);
	}
}

TEST(TestProtocolResultPreparedMetadata, encode) {
	{
		cql::ProtocolResultPreparedMetadata value;
		value.setPartitionKeyIndexes({ cql::ProtocolShort(1) });
		value.setColumns({ cql::ProtocolResultColumn(), cql::ProtocolResultColumn() });
		value.getColumns().at(0).setKeySpace("k");
		value.getColumns().at(0).setTable("t");
		value.getColumns().at(0).setName("a");
		value.getColumns().at(0).setType(cql::ProtocolColumnOption(cql::ColumnType::Int));
		value.getColumns().at(1).setKeySpace("K");
		value.getColumns().at(1).setTable("T");
		value.getColumns().at(1).setName("b");
		value.getColumns().at(1).setType(cql::ProtocolColumnOption(cql::ColumnType::Ascii));
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x00"
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x01"
			"\x00\x01"
			"\x00\x01""k""\x00\x01""t""\x00\x01""a""\x00\x09"
			"\x00\x01""K""\x00\x01""T""\x00\x01""b""\x00\x01"));
	}
	{
		cql::ProtocolResultPreparedMetadata value;
		value.setGlobalKeySpaceAndTable("k0", "t0");
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
			"\x00\x00\x00\x00"
			"\x00\x02""k0""\x00\x02""t0"
			"\x00\x01""a""\x00\x09"
			"\x00\x01""b""\x00\x01"));
	}
}

TEST(TestProtocolResultPreparedMetadata, decode) {
	cql::ProtocolResultPreparedMetadata value;
	{
		auto data = makeTestString(
			"\x00\x00\x00\x00"
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x01"
			"\x00\x01"
			"\x00\x01""k""\x00\x01""t""\x00\x01""a""\x00\x09"
			"\x00\x01""K""\x00\x01""T""\x00\x01""b""\x00\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getFlags(), cql::ResultPreparedMetadataFlags::None);
		ASSERT_EQ(value.getPartitionKeyIndexes().size(), 1U);
		ASSERT_EQ(value.getPartitionKeyIndexes().at(0).get(), 1);
		ASSERT_EQ(value.getColumns().size(), 2U);
		ASSERT_EQ(value.getColumns().at(0).getKeySpace(), "k");
		ASSERT_EQ(value.getColumns().at(0).getTable(), "t");
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::ColumnType::Int);
		ASSERT_EQ(value.getColumns().at(1).getKeySpace(), "K");
		ASSERT_EQ(value.getColumns().at(1).getTable(), "T");
		ASSERT_EQ(value.getColumns().at(1).getName(), "b");
		ASSERT_EQ(value.getColumns().at(1).getType().get(), cql::ColumnType::Ascii);
	}
	{
		auto data = makeTestString(
			"\x00\x00\x00\x01"
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x00"
			"\x00\x02""k0""\x00\x02""t0"
			"\x00\x01""a""\x00\x09"
			"\x00\x01""b""\x00\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getFlags(), cql::ResultPreparedMetadataFlags::GlobalTableSpec);
		ASSERT_TRUE(value.getPartitionKeyIndexes().empty());
		ASSERT_EQ(value.getGlobalKeySpace(), "k0");
		ASSERT_EQ(value.getGlobalTable(), "t0");
		ASSERT_EQ(value.getColumns().size(), 2U);
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::ColumnType::Int);
		ASSERT_EQ(value.getColumns().at(1).getName(), "b");
		ASSERT_EQ(value.getColumns().at(1).getType().get(), cql::ColumnType::Ascii);
	}
}

