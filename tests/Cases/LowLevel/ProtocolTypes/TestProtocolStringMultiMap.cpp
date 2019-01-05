#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolStringMultiMap.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolStringMultiMap, getset) {
	cql::ProtocolStringMultiMap value({
		{
			cql::ProtocolString("apple"),
			cql::ProtocolStringList({
				cql::ProtocolString("dogA"),
				cql::ProtocolString("dogB")
			})
		},
		{
			cql::ProtocolString("orange"),
			cql::ProtocolStringList({
				cql::ProtocolString("cat")
			})
		}
	});
	ASSERT_EQ(value.get().size(), 2U);
	ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get().size(), 2U);
	ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get().at(0).get(), "dogA");
	ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get().at(1).get(), "dogB");
	ASSERT_EQ(value.get().at(cql::ProtocolString("orange")).get().size(), 1U);
	ASSERT_EQ(value.get().at(cql::ProtocolString("orange")).get().at(0).get(), "cat");

	value.get().emplace(
		cql::ProtocolString("banana"),
		cql::ProtocolStringList({ cql::ProtocolString("cow") }));
	ASSERT_EQ(value.get().size(), 3U);

	value = cql::ProtocolStringMultiMap();
	ASSERT_TRUE(value.get().empty());
}

TEST(TestProtocolStringMultiMap, encode) {
	cql::ProtocolStringMultiMap value({
		{
			cql::ProtocolString("apple"),
			cql::ProtocolStringList({
				cql::ProtocolString("dogA"),
				cql::ProtocolString("dogB")
			})
		},
		{
			cql::ProtocolString("orange"),
			cql::ProtocolStringList({
				cql::ProtocolString("cat")
			})
		}
	});
	std::string data;
	value.encode(data);
	auto encodedA = makeTestString(
		"\x00\x02"
		"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB"
		"\x00\x06""orange""\x00\x01""\x00\x03""cat");
	auto encodedB = makeTestString(
		"\x00\x02"
		"\x00\x06""orange""\x00\x01""\x00\x03""cat"
		"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB");
	ASSERT_TRUE(data == encodedA || data == encodedB);
}

TEST(TestProtocolStringMultiMap, decode) {
	cql::ProtocolStringMultiMap value;
	{
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB"
			"\x00\x06""orange""\x00\x01""\x00\x03""cat");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 2U);
		ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get().size(), 2U);
		ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get().at(0).get(), "dogA");
		ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get().at(1).get(), "dogB");
		ASSERT_EQ(value.get().at(cql::ProtocolString("orange")).get().size(), 1U);
		ASSERT_EQ(value.get().at(cql::ProtocolString("orange")).get().at(0).get(), "cat");
	}
	{
		auto data = makeTestString(
			"\x00\x01"
			"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 1U);
		ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get().size(), 2U);
		ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get().at(0).get(), "dogA");
		ASSERT_EQ(value.get().at(cql::ProtocolString("apple")).get().at(1).get(), "dogB");
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

TEST(TestProtocolStringMultiMap, decodeError) {
	{
		cql::ProtocolStringMultiMap value;
		auto data = makeTestString("\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolStringMultiMap value;
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolStringMultiMap value;
		auto data = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB"
			"\x00\x06""orange""\x00\x01""\x00\x03");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

