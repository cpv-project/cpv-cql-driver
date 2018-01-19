#include <LowLevel/ProtocolTypes/CqlProtocolResultPreparedMetadata.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolResultPreparedMetadata, getset) {
	{
		cql::CqlProtocolResultPreparedMetadata value;
		value.setPartitionKeyIndexes({ cql::CqlProtocolInt(1) });
		value.setColumns({ cql::CqlProtocolResultColumn(), cql::CqlProtocolResultColumn() });
		value.getColumns().at(0).setKeySpace("k");
		value.getColumns().at(0).setTable("t");
		value.getColumns().at(0).setName("a");
		value.getColumns().at(0).setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Int));
		value.getColumns().at(1).setKeySpace("K");
		value.getColumns().at(1).setTable("T");
		value.getColumns().at(1).setName("b");
		value.getColumns().at(1).setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Ascii));
		ASSERT_EQ(value.getFlags(), cql::CqlResultPreparedMetadataFlags::None);
		ASSERT_EQ(value.getPartitionKeyIndexes().size(), 1);
		ASSERT_EQ(value.getPartitionKeyIndexes().at(0).get(), 1);
		ASSERT_EQ(value.getColumns().size(), 2);
		ASSERT_EQ(value.getColumns().at(0).getKeySpace(), "k");
		ASSERT_EQ(value.getColumns().at(0).getTable(), "t");
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::CqlColumnType::Int);
		ASSERT_EQ(value.getColumns().at(1).getKeySpace(), "K");
		ASSERT_EQ(value.getColumns().at(1).getTable(), "T");
		ASSERT_EQ(value.getColumns().at(1).getName(), "b");
		ASSERT_EQ(value.getColumns().at(1).getType().get(), cql::CqlColumnType::Ascii);
	}
	{
		cql::CqlProtocolResultPreparedMetadata value;
		value.setGlobalKeySpaceAndTable("k0", "t0");
		value.setColumns({ cql::CqlProtocolResultColumn(), cql::CqlProtocolResultColumn() });
		value.getColumns().at(0).setName("a");
		value.getColumns().at(0).setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Int));
		value.getColumns().at(1).setName("b");
		value.getColumns().at(1).setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Ascii));
		ASSERT_EQ(value.getFlags(), cql::CqlResultPreparedMetadataFlags::GlobalTableSpec);
		ASSERT_TRUE(value.getPartitionKeyIndexes().empty());
		ASSERT_EQ(value.getGlobalKeySpace(), "k0");
		ASSERT_EQ(value.getGlobalTable(), "t0");
		ASSERT_EQ(value.getColumns().size(), 2);
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::CqlColumnType::Int);
		ASSERT_EQ(value.getColumns().at(1).getName(), "b");
		ASSERT_EQ(value.getColumns().at(1).getType().get(), cql::CqlColumnType::Ascii);
	}
}

TEST(TestCqlProtocolResultPreparedMetadata, encode) {
	{
		cql::CqlProtocolResultPreparedMetadata value;
		value.setPartitionKeyIndexes({ cql::CqlProtocolInt(1) });
		value.setColumns({ cql::CqlProtocolResultColumn(), cql::CqlProtocolResultColumn() });
		value.getColumns().at(0).setKeySpace("k");
		value.getColumns().at(0).setTable("t");
		value.getColumns().at(0).setName("a");
		value.getColumns().at(0).setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Int));
		value.getColumns().at(1).setKeySpace("K");
		value.getColumns().at(1).setTable("T");
		value.getColumns().at(1).setName("b");
		value.getColumns().at(1).setType(cql::CqlProtocolColumnOption(cql::CqlColumnType::Ascii));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x00"
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x01"
			"\x00\x00\x00\x01"
			"\x00\x01""k""\x00\x01""t""\x00\x01""a""\x00\x09"
			"\x00\x01""K""\x00\x01""T""\x00\x01""b""\x00\x01"));
	}
	{
		cql::CqlProtocolResultPreparedMetadata value;
		value.setGlobalKeySpaceAndTable("k0", "t0");
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
			"\x00\x00\x00\x00"
			"\x00\x02""k0""\x00\x02""t0"
			"\x00\x01""a""\x00\x09"
			"\x00\x01""b""\x00\x01"));
	}
}

TEST(TestCqlProtocolResultPreparedMetadata, decode) {
	cql::CqlProtocolResultPreparedMetadata value;
	{
		auto data = makeTestString(
			"\x00\x00\x00\x00"
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x01"
			"\x00\x00\x00\x01"
			"\x00\x01""k""\x00\x01""t""\x00\x01""a""\x00\x09"
			"\x00\x01""K""\x00\x01""T""\x00\x01""b""\x00\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getFlags(), cql::CqlResultPreparedMetadataFlags::None);
		ASSERT_EQ(value.getPartitionKeyIndexes().size(), 1);
		ASSERT_EQ(value.getPartitionKeyIndexes().at(0).get(), 1);
		ASSERT_EQ(value.getColumns().size(), 2);
		ASSERT_EQ(value.getColumns().at(0).getKeySpace(), "k");
		ASSERT_EQ(value.getColumns().at(0).getTable(), "t");
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::CqlColumnType::Int);
		ASSERT_EQ(value.getColumns().at(1).getKeySpace(), "K");
		ASSERT_EQ(value.getColumns().at(1).getTable(), "T");
		ASSERT_EQ(value.getColumns().at(1).getName(), "b");
		ASSERT_EQ(value.getColumns().at(1).getType().get(), cql::CqlColumnType::Ascii);
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
		ASSERT_EQ(value.getFlags(), cql::CqlResultPreparedMetadataFlags::GlobalTableSpec);
		ASSERT_TRUE(value.getPartitionKeyIndexes().empty());
		ASSERT_EQ(value.getGlobalKeySpace(), "k0");
		ASSERT_EQ(value.getGlobalTable(), "t0");
		ASSERT_EQ(value.getColumns().size(), 2);
		ASSERT_EQ(value.getColumns().at(0).getName(), "a");
		ASSERT_EQ(value.getColumns().at(0).getType().get(), cql::CqlColumnType::Int);
		ASSERT_EQ(value.getColumns().at(1).getName(), "b");
		ASSERT_EQ(value.getColumns().at(1).getType().get(), cql::CqlColumnType::Ascii);
	}
}

