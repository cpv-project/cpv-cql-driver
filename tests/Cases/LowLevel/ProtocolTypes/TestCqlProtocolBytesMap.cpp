#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolBytesMap.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolBytesMap, getset) {
	cql::CqlProtocolBytesMap value({
		{ cql::CqlProtocolString("apple"), cql::CqlProtocolBytes("dog") },
		{ cql::CqlProtocolString("orange"), cql::CqlProtocolBytes("cat") }
	});
	ASSERT_EQ(value.get().size(), 2);
	ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get(), "dog");
	ASSERT_EQ(value.get().at(cql::CqlProtocolString("orange")).get(), "cat");

	value.get().emplace(cql::CqlProtocolString("banana"), cql::CqlProtocolBytes("cow"));
	ASSERT_EQ(value.get().size(), 3);

	value = cql::CqlProtocolBytesMap();
	ASSERT_TRUE(value.get().empty());
}

TEST(TestCqlProtocolBytesMap, encode) {
	cql::CqlProtocolBytesMap value({
		{ cql::CqlProtocolString("apple"), cql::CqlProtocolBytes("dog") },
		{ cql::CqlProtocolString("orange"), cql::CqlProtocolBytes("cat") }
	});
	seastar::sstring data;
	value.encode(data);
	auto encodedA = makeTestString(
		"\x00\x02"
		"\x00\x05""apple""\x00\x00\x00\x03""dog"
		"\x00\x06""orange""\x00\x00\x00\x03""cat");
	auto encodedB = makeTestString(
		"\x00\x02"
		"\x00\x06""orange""\x00\x00\x00\x03""cat"
		"\x00\x05""apple""\x00\x00\x00\x03""dog");
	ASSERT_TRUE(data == encodedA || data == encodedB);
}

TEST(TestCqlProtocolBytesMap, decode) {
	cql::CqlProtocolBytesMap value;
	{
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x00\x00\x03""dog"
			"\x00\x06""orange""\x00\x00\x00\x03""cat");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 2);
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get(), "dog");
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("orange")).get(), "cat");
	}
	{
		auto data = makeTestString(
			"\x00\x01"
			"\x00\x05""apple""\x00\x00\x00\x03""dog");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 1);
		ASSERT_EQ(value.get().at(cql::CqlProtocolString("apple")).get(), "dog");
	}
	{
		auto data = makeTestString(
			"\x00\x01"
			"\x00\x05""apple""\xff\xff\xff\xff");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 1);
		ASSERT_TRUE(value.get().at(cql::CqlProtocolString("apple")).isNull());
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

TEST(TestCqlProtocolBytesMap, decodeError) {
	{
		cql::CqlProtocolBytesMap value;
		auto data = makeTestString("\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolBytesMap value;
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x00\x00\x03""dog");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolBytesMap value;
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x00\x00\x03""dog"
			"\x00\x06""orange""\x00\x00\x00\x03""ca");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

