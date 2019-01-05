#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolValueMap.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolValueMap, getset) {
	cql::ProtocolValueMap value({
		{ cql::ProtocolString("apple"), cql::ProtocolValue("dog") },
		{ cql::ProtocolString("orange"), cql::ProtocolValue("cat") }
	});
	ASSERT_EQ(value.get().size(), 2U);
	ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get(), "dog");
	ASSERT_EQ(value.get().at(cql::ProtocolString("orange")).get(), "cat");

	value.get().emplace(cql::ProtocolString("banana"), cql::ProtocolValue("cow"));
	ASSERT_EQ(value.get().size(), 3U);

	value = cql::ProtocolValueMap();
	ASSERT_TRUE(value.get().empty());
}

TEST(TestProtocolValueMap, encode) {
	cql::ProtocolValueMap value({
		{ cql::ProtocolString("apple"), cql::ProtocolValue("dog") },
		{ cql::ProtocolString("orange"), cql::ProtocolValue("cat") }
	});
	std::string data;
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

TEST(TestProtocolValueMap, decode) {
	cql::ProtocolValueMap value;
	{
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x00\x00\x03""dog"
			"\x00\x06""orange""\x00\x00\x00\x03""cat");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 2U);
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
		ASSERT_EQ(value.get().size(), 1U);
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

TEST(TestProtocolValueMap, decodeError) {
	{
		cql::ProtocolValueMap value;
		auto data = makeTestString("\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolValueMap value;
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x00\x00\x03""dog");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolValueMap value;
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x00\x00\x03""dog"
			"\x00\x06""orange""\x00\x00\x00\x03""c");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

