#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolValueList.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolValueList, getset) {
	cql::CqlProtocolValueList value({
		cql::CqlProtocolValue("abc"),
		cql::CqlProtocolValue("aaaaa")
	});
	ASSERT_EQ(value.get().size(), 2);
	ASSERT_EQ(value.get().at(0).get(), "abc");
	ASSERT_EQ(value.get().at(1).get(), "aaaaa");

	value.get().emplace_back("asd");
	ASSERT_EQ(value.get().size(), 3);

	value = cql::CqlProtocolValueList();
	ASSERT_TRUE(value.get().empty());
}

TEST(TestCqlProtocolValueList, encode) {
	cql::CqlProtocolValueList value({
		cql::CqlProtocolValue("abc"),
		cql::CqlProtocolValue("aaaaa")
	});
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x02""\x00\x00\x00\x03""abc""\x00\x00\x00\x05""aaaaa"));
}

TEST(TestCqlProtocolValueList, decode) {
	cql::CqlProtocolValueList value;
	{
		auto data = makeTestString("\x00\x02""\x00\x00\x00\x03""abc""\x00\x00\x00\x05""aaaaa");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 2);
		ASSERT_EQ(value.get().at(0).get(), "abc");
		ASSERT_EQ(value.get().at(1).get(), "aaaaa");
	}
	{
		auto data = makeTestString("\x00\x01""\x00\x00\x00\x02""aa");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 1);
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
		cql::CqlProtocolValueList longList;
		for (std::size_t i = 1, j = cql::CqlProtocolValueList::SmallSizeBoundary + 3; i < j; ++i) {
			longList.get().emplace_back(
				seastar::sstring(reinterpret_cast<const char*>(&i), sizeof(i)));
		}
		seastar::sstring data;
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

TEST(TestCqlProtocolValueList, decodeError) {
	{
		cql::CqlProtocolValueList value;
		auto data = makeTestString("\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolValueList value;
		auto data = makeTestString("\x01\x02""\x00\x00\x00\x03""abc");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolValueList value;
		auto data = makeTestString("\x01\x02""\x00\x00\x00\x03""abc""\x00\x00\x00\x05""aaaa");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

