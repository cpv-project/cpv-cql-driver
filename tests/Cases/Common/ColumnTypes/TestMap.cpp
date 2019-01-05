#include <CQLDriver/Common/ColumnTypes/Map.hpp>
#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <CQLDriver/Common/ColumnTypes/BigInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestMap, getset) {
	using MapType = std::unordered_map<
		cql::Text, cql::BigInt, cql::ColumnTrait<cql::Text>::Hash>;
	cql::Map<cql::Text, cql::BigInt> value;
	value.set(MapType({ { "abc", 123 } }));
	ASSERT_EQ(value.get().size(), 1U);
	ASSERT_EQ(value.get().at("abc"), 123);

	value.set({ { "asd", 100 }, { "qwe", 101 } });
	ASSERT_EQ(value.get().size(), 2U);
	ASSERT_EQ(value.get().at("asd"), 100);
	ASSERT_EQ(value.get().at("qwe"), 101);
}

TEST(TestMap, encodeBody) {
	{
		cql::Map<cql::Text, cql::BigInt> value;
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));
	}
	{
		cql::Map<cql::Text, cql::BigInt> value({ { "abc", 123 } });
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x01"
			"\x00\x00\x00\x03""abc"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x7b"));
	}
}

TEST(TestMap, decodeBody) {
	{
		cql::Map<cql::Text, cql::BigInt> value({ { "abc", 123 } });
		auto data = makeTestString("\x00\x00\x00\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_TRUE(value->empty());
	}
	{
		cql::Map<cql::Text, cql::BigInt> value;
		auto data = makeTestString(
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x03""asd"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x64"
			"\x00\x00\x00\x03""qwe"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x65");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value->size(), 2U);
		ASSERT_EQ(value->at("asd"), 100);
		ASSERT_EQ(value->at("qwe"), 101);
	}
}

TEST(TestMap, decodeBodyError) {
	{
		cql::Map<cql::Text, cql::BigInt> value;
		auto data = makeTestString("\x00\x00\x00");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of map size not enough");
	}
	{
		cql::Map<cql::Text, cql::BigInt> value;
		auto data = makeTestString("\xff\xff\xff\xff");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"map size can't be negative");
	}
	{
		cql::Map<cql::Text, cql::BigInt> value;
		auto data = makeTestString(
			"\x00\x00\x00\x01"
			"\x00\x00\x00\x03""abc"
			"\x00\x00\x00\x01\xff");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"integer length not matched");
	}
}

TEST(TestMap, operations) {
	{
		// construct with rvalue initializer list
		std::string keyA("a");
		std::string keyB("b");
		cql::Map<cql::Text, cql::BigInt> value({ { keyA, 1 }, { std::move(keyB), 2 } });
		ASSERT_EQ(keyA, "a");
		ASSERT_EQ(keyB, "");
	}
	{
		// dereference
		cql::Map<cql::Text, cql::BigInt> value({ { "abc", 123 } });
		ASSERT_EQ((*value).size(), 1U);
		ASSERT_EQ((*value).at("abc"), 123);
	}
	{
		// get pointer
		cql::Map<cql::Text, cql::BigInt> value;
		value->emplace("abc", 123);
		ASSERT_EQ(value->size(), 1U);
		ASSERT_EQ(value->at("abc"), 123);
	}
}

