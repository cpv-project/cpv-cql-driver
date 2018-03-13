#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolColumnOptionList.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolColumnOptionList, getset) {
	cql::ProtocolColumnOptionList value({
		cql::ProtocolColumnOption(cql::ColumnType::Ascii),
		cql::ProtocolColumnOption(cql::ColumnType::Int)
	});
	ASSERT_EQ(value.get().size(), 2);
	ASSERT_EQ(value.get().at(0).get(), cql::ColumnType::Ascii);
	ASSERT_EQ(value.get().at(1).get(), cql::ColumnType::Int);

	value.get().emplace_back(cql::ProtocolColumnOption(cql::ColumnType::VarChar));
	ASSERT_EQ(value.get().size(), 3);

	value = cql::ProtocolColumnOptionList();
	ASSERT_TRUE(value.get().empty());
}

TEST(TestProtocolColumnOptionList, encode) {
	cql::ProtocolColumnOptionList value({
		cql::ProtocolColumnOption(cql::ColumnType::Ascii),
		cql::ProtocolColumnOption(cql::ColumnType::Int)
	});
	std::string data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x02\x00\x01\x00\x09"));
}

TEST(TestProtocolColumnOptionList, decode) {
	cql::ProtocolColumnOptionList value;
	{
		auto data = makeTestString("\x00\x02\x00\x01\x00\x09");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 2);
		ASSERT_EQ(value.get().at(0).get(), cql::ColumnType::Ascii);
		ASSERT_EQ(value.get().at(1).get(), cql::ColumnType::Int);
	}
	{
		auto data = makeTestString("\x00\x01\x00\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 1);
		ASSERT_EQ(value.get().at(0).get(), cql::ColumnType::Ascii);
	}
	{
		auto data = makeTestString("\x00\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_TRUE(value.get().empty());
	}
}

TEST(TestProtocolColumnOptionList, decodeError) {
	{
		cql::ProtocolColumnOptionList value;
		auto data = makeTestString("\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolColumnOptionList value;
		auto data = makeTestString("\x00\x02\x00\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolColumnOptionList value;
		auto data = makeTestString("\x00\x01\x00\x20");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

