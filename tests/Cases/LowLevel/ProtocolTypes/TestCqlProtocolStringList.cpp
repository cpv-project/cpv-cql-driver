#include <CqlDriver/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolStringList.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolStringList, getset) {
	cql::CqlProtocolStringList value({
		cql::CqlProtocolString("abc"),
		cql::CqlProtocolString("aaaaa")
	});
	ASSERT_EQ(value.get().size(), 2);
	ASSERT_EQ(value.get().at(0).get(), "abc");
	ASSERT_EQ(value.get().at(1).get(), "aaaaa");

	value.get().emplace_back("asd");
	ASSERT_EQ(value.get().size(), 3);

	value = cql::CqlProtocolStringList();
	ASSERT_TRUE(value.get().empty());
}

TEST(TestCqlProtocolStringList, encode) {
	cql::CqlProtocolStringList value({
		cql::CqlProtocolString("abc"),
		cql::CqlProtocolString("aaaaa")
	});
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, seastar::sstring("\x00\x02""\x00\x03""abc""\x00\x05""aaaaa", 14));
}

TEST(TestCqlProtocolStringList, decode) {
	cql::CqlProtocolStringList value;
	{
		seastar::sstring data("\x00\x02""\x00\x03""abc""\x00\x05""aaaaa", 14);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_EQ(value.get().size(), 2);
		ASSERT_EQ(value.get().at(0).get(), "abc");
		ASSERT_EQ(value.get().at(1).get(), "aaaaa");
	}
	{
		seastar::sstring data("\x00\x01""\x00\x02""aa", 6);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_EQ(value.get().size(), 1);
		ASSERT_EQ(value.get().at(0).get(), "aa");
	}
	{
		seastar::sstring data("\x00\x00", 2);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(value.get().empty());
	}
	{
		cql::CqlProtocolStringList longList;
		for (std::size_t i = 1, j = cql::CqlProtocolStringList::SmallSizeBoundary + 3; i < j; ++i) {
			longList.get().emplace_back(
				seastar::sstring(reinterpret_cast<const char*>(&i), sizeof(i)));
		}
		seastar::sstring data;
		longList.encode(data);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_EQ(value.get().size(), longList.get().size());
		for (std::size_t i = 1, j = longList.get().size(); i < j; ++i) {
			ASSERT_EQ(value.get()[i].get(), longList.get()[i].get());
		}
	}
}

TEST(TestCqlProtocolStringList, decodeError) {
	{
		cql::CqlProtocolStringList value;
		seastar::sstring data("\x01", 1);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolStringList value;
		seastar::sstring data("\x01\x02""\x00\x03""abc", 7);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolStringList value;
		seastar::sstring data("\x01\x02""\x00\x03""abc""\x00\x05""aaaa", 13);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

