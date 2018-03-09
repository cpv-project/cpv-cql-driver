#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolStringMap.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolStringMap, getset) {
	cql::ProtocolStringMap value({
		{ cql::ProtocolString("apple"), cql::ProtocolString("dog") },
		{ cql::ProtocolString("orange"), cql::ProtocolString("cat") }
	});
	ASSERT_EQ(value.get().size(), 2);
	ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get(), "dog");
	ASSERT_EQ(value.get().at(cql::ProtocolString("orange")).get(), "cat");

	value.get().emplace(cql::ProtocolString("banana"), cql::ProtocolString("cow"));
	ASSERT_EQ(value.get().size(), 3);

	value = cql::ProtocolStringMap();
	ASSERT_TRUE(value.get().empty());
}

TEST(TestProtocolStringMap, encode) {
	cql::ProtocolStringMap value({
		{ cql::ProtocolString("apple"), cql::ProtocolString("dog") },
		{ cql::ProtocolString("orange"), cql::ProtocolString("cat") }
	});
	seastar::sstring data;
	value.encode(data);
	auto encodedA = makeTestString(
		"\x00\x02"
		"\x00\x05""apple""\x00\x03""dog"
		"\x00\x06""orange""\x00\x03""cat");
	auto encodedB = makeTestString(
		"\x00\x02"
		"\x00\x06""orange""\x00\x03""cat"
		"\x00\x05""apple""\x00\x03""dog");
	ASSERT_TRUE(data == encodedA || data == encodedB);
}

TEST(TestProtocolStringMap, decode) {
	cql::ProtocolStringMap value;
	{
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x03""dog"
			"\x00\x06""orange""\x00\x03""cat");
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
			"\x00\x05""apple""\x00\x03""dog");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 1);
		ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get(), "dog");
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

TEST(TestProtocolStringMap, decodeError) {
	{
		cql::ProtocolStringMap value;
		auto data = makeTestString("\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolStringMap value;
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x03""dog");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolStringMap value;
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x03""dog"
			"\x00\x06""orange""\x00\x03""c");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

