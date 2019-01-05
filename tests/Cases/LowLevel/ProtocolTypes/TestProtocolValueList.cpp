#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolValueList.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolValueList, getset) {
	cql::ProtocolValueList value({
		cql::ProtocolValue("abc"),
		cql::ProtocolValue("aaaaa")
	});
	ASSERT_EQ(value.get().size(), 2U);
	ASSERT_EQ(value.get().at(0).get(), "abc");
	ASSERT_EQ(value.get().at(1).get(), "aaaaa");

	value.get().emplace_back("asd");
	ASSERT_EQ(value.get().size(), 3U);

	value = cql::ProtocolValueList();
	ASSERT_TRUE(value.get().empty());
}

TEST(TestProtocolValueList, encode) {
	cql::ProtocolValueList value({
		cql::ProtocolValue("abc"),
		cql::ProtocolValue("aaaaa")
	});
	std::string data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x02""\x00\x00\x00\x03""abc""\x00\x00\x00\x05""aaaaa"));
}

TEST(TestProtocolValueList, decode) {
	cql::ProtocolValueList value;
	{
		auto data = makeTestString("\x00\x02""\x00\x00\x00\x03""abc""\x00\x00\x00\x05""aaaaa");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 2U);
		ASSERT_EQ(value.get().at(0).get(), "abc");
		ASSERT_EQ(value.get().at(1).get(), "aaaaa");
	}
	{
		auto data = makeTestString("\x00\x01""\x00\x00\x00\x02""aa");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 1U);
		ASSERT_EQ(value.get().at(0).get(), "aa");
	}
	{
		auto data = makeTestString("\x00\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_TRUE(value.get().empty());
	}
	{
		cql::ProtocolValueList longList;
		for (std::size_t i = 1, j = cql::ProtocolValueList::SmallSizeBoundary + 3; i < j; ++i) {
			longList.get().emplace_back(
				std::string(reinterpret_cast<const char*>(&i), sizeof(i)));
		}
		std::string data;
		longList.encode(data);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), longList.get().size());
		for (std::size_t i = 1, j = longList.get().size(); i < j; ++i) {
			ASSERT_EQ(value.get()[i].get(), longList.get()[i].get());
		}
	}
}

TEST(TestProtocolValueList, decodeError) {
	{
		cql::ProtocolValueList value;
		auto data = makeTestString("\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolValueList value;
		auto data = makeTestString("\x01\x02""\x00\x00\x00\x03""abc");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolValueList value;
		auto data = makeTestString("\x01\x02""\x00\x00\x00\x03""abc""\x00\x00\x00\x05""aaaa");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

