#include <CqlDriver/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolStringMap.hpp>
#include <TestUtility/GTestUtils.hpp>


TEST(TestCqlProtocolStringMap, getset) {
	cql::CqlProtocolStringMap value({
		{ cql::CqlProtocolString("apple"), cql::CqlProtocolString("dog") },
		{ cql::CqlProtocolString("orange"), cql::CqlProtocolString("cat") }
	});
	ASSERT_EQ(value.get().size(), 2);
	ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get(), "dog");
	ASSERT_EQ(value.get().at(cql::CqlProtocolString("orange")).get(), "cat");

	value.get().emplace(cql::CqlProtocolString("banana"), cql::CqlProtocolString("cow"));
	ASSERT_EQ(value.get().size(), 3);

	value = cql::CqlProtocolStringMap();
	ASSERT_TRUE(value.get().empty());
}

TEST(TestCqlProtocolStringMap, encode) {
	cql::CqlProtocolStringMap value({
		{ cql::CqlProtocolString("apple"), cql::CqlProtocolString("dog") },
		{ cql::CqlProtocolString("orange"), cql::CqlProtocolString("cat") }
	});
	seastar::sstring data;
	value.encode(data);
	seastar::sstring encodedA(
		"\x00\x02"
		"\x00\x05""apple""\x00\x03""dog"
		"\x00\x06""orange""\x00\x03""cat" , 27);
	seastar::sstring encodedB(
		"\x00\x02"
		"\x00\x06""orange""\x00\x03""cat"
		"\x00\x05""apple""\x00\x03""dog", 27);
	ASSERT_TRUE(data == encodedA || data == encodedB);
}

TEST(TestCqlProtocolStringMap, decode) {
	cql::CqlProtocolStringMap value;
	{
		seastar::sstring data(
			"\x00\x02"
			"\x00\x05""apple""\x00\x03""dog"
			"\x00\x06""orange""\x00\x03""cat" , 27);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_EQ(value.get().size(), 2);
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get(), "dog");
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("orange")).get(), "cat");
	}
	{
		seastar::sstring data(
			"\x00\x01"
			"\x00\x05""apple""\x00\x03""dog", 14);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_EQ(value.get().size(), 1);
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get(), "dog");
	}
	{
		seastar::sstring data("\x00\x00", 2);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(value.get().empty());
	}
}

TEST(TestCqlProtocolStringMap, decodeError) {
	{
		cql::CqlProtocolStringMap value;
		seastar::sstring data("\x01", 1);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolStringMap value;
		seastar::sstring data(
			"\x00\x02"
			"\x00\x05""apple""\x00\x03""dog", 14);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolStringMap value;
		seastar::sstring data(
			"\x00\x02"
			"\x00\x05""apple""\x00\x03""dog"
			"\x00\x06""orange""\x00\x03""c" , 25);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

