#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolBytesMap.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolBytesMap, getset) {
	cql::ProtocolBytesMap value({
		{ cql::ProtocolString("apple"), cql::ProtocolBytes("dog") },
		{ cql::ProtocolString("orange"), cql::ProtocolBytes("cat") }
	});
	ASSERT_EQ(value.get().size(), 2);
	ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get(), "dog");
	ASSERT_EQ(value.get().at(cql::ProtocolString("orange")).get(), "cat");

	value.get().emplace(cql::ProtocolString("banana"), cql::ProtocolBytes("cow"));
	ASSERT_EQ(value.get().size(), 3);

	value = cql::ProtocolBytesMap();
	ASSERT_TRUE(value.get().empty());
}

TEST(TestProtocolBytesMap, encode) {
	cql::ProtocolBytesMap value({
		{ cql::ProtocolString("apple"), cql::ProtocolBytes("dog") },
		{ cql::ProtocolString("orange"), cql::ProtocolBytes("cat") }
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

TEST(TestProtocolBytesMap, decode) {
	cql::ProtocolBytesMap value;
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
		ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get(), "dog");
		ASSERT_EQ(value.get().at(cql::ProtocolString("orange")).get(), "cat");
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
		ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get(), "dog");
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
		ASSERT_TRUE(value.get().at(cql::ProtocolString("apple")).isNull());
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

TEST(TestProtocolBytesMap, decodeError) {
	{
		cql::ProtocolBytesMap value;
		auto data = makeTestString("\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolBytesMap value;
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x00\x00\x03""dog");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolBytesMap value;
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x00\x00\x03""dog"
			"\x00\x06""orange""\x00\x00\x00\x03""ca");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

