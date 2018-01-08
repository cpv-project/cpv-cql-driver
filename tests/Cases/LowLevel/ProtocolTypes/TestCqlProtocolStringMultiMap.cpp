#include <CqlDriver/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolStringMultiMap.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolStringMultiMap, getset) {
	cql::CqlProtocolStringMultiMap value({
		{
			cql::CqlProtocolString("apple"),
			cql::CqlProtocolStringList({
				cql::CqlProtocolString("dogA"),
				cql::CqlProtocolString("dogB")
			})
		},
		{
			cql::CqlProtocolString("orange"),
			cql::CqlProtocolStringList({
				cql::CqlProtocolString("cat")
			})
		}
	});
	ASSERT_EQ(value.get().size(), 2);
	ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get().size(), 2);
	ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get().at(0).get(), "dogA");
	ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get().at(1).get(), "dogB");
	ASSERT_EQ(value.get().at(cql::CqlProtocolString("orange")).get().size(), 1);
	ASSERT_EQ(value.get().at(cql::CqlProtocolString("orange")).get().at(0).get(), "cat");

	value.get().emplace(
		cql::CqlProtocolString("banana"),
		cql::CqlProtocolStringList({ cql::CqlProtocolString("cow") }));
	ASSERT_EQ(value.get().size(), 3);

	value = cql::CqlProtocolStringMultiMap();
	ASSERT_TRUE(value.get().empty());
}

TEST(TestCqlProtocolStringMultiMap, encode) {
	cql::CqlProtocolStringMultiMap value({
		{
			cql::CqlProtocolString("apple"),
			cql::CqlProtocolStringList({
				cql::CqlProtocolString("dogA"),
				cql::CqlProtocolString("dogB")
			})
		},
		{
			cql::CqlProtocolString("orange"),
			cql::CqlProtocolStringList({
				cql::CqlProtocolString("cat")
			})
		}
	});
	seastar::sstring data;
	value.encode(data);
	seastar::sstring encodedA(
		"\x00\x02"
		"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB"
		"\x00\x06""orange""\x00\x01""\x00\x03""cat" , 38);
	seastar::sstring encodedB(
		"\x00\x02"
		"\x00\x06""orange""\x00\x01""\x00\x03""cat"
		"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB", 38);
	ASSERT_TRUE(data == encodedA || data == encodedB);
}

TEST(TestCqlProtocolStringMultiMap, decode) {
	cql::CqlProtocolStringMultiMap value;
	{
		seastar::sstring data(
			"\x00\x02"
			"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB"
			"\x00\x06""orange""\x00\x01""\x00\x03""cat" , 38);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_EQ(value.get().size(), 2);
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get().size(), 2);
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get().at(0).get(), "dogA");
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get().at(1).get(), "dogB");
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("orange")).get().size(), 1);
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("orange")).get().at(0).get(), "cat");
	}
	{
		seastar::sstring data(
			"\x00\x01"
			"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB", 23);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_EQ(value.get().size(), 1);
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get().size(), 2);
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get().at(0).get(), "dogA");
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get().at(1).get(), "dogB");
	}
	{
		seastar::sstring data("\x00\x00", 2);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(value.get().empty());
	}
}

TEST(TestCqlProtocolStringMultiMap, decodeError) {
	{
		cql::CqlProtocolStringMultiMap value;
		seastar::sstring data("\x01", 1);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolStringMultiMap value;
		seastar::sstring data(
			"\x00\x02"
			"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB", 23);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolStringMultiMap value;
		seastar::sstring data(
			"\x00\x02"
			"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB"
			"\x00\x06""orange""\x00\x01""\x00\x03" , 35);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

