#include <CqlDriver/Common/ColumnTypes/CqlMap.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlText.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlBigInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlMap, getset) {
	using MapType = std::unordered_map<
		cql::CqlText, cql::CqlBigInt, cql::CqlColumnTrait<cql::CqlText>::Hash>;
	cql::CqlMap<cql::CqlText, cql::CqlBigInt> value;
	value.set(MapType({ { "abc", 123 } }));
	ASSERT_EQ(value.get().size(), 1);
	ASSERT_EQ(value.get().at("abc"), 123);

	value.set({ { "asd", 100 }, { "qwe", 101 } });
	ASSERT_EQ(value.get().size(), 2);
	ASSERT_EQ(value.get().at("asd"), 100);
	ASSERT_EQ(value.get().at("qwe"), 101);
}

TEST(TestCqlMap, encodeBody) {
	{
		cql::CqlMap<cql::CqlText, cql::CqlBigInt> value;
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));
	}
	{
		cql::CqlMap<cql::CqlText, cql::CqlBigInt> value({ { "abc", 123 } });
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x01"
			"\x00\x00\x00\x03""abc"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x7b"));
	}
}

TEST(TestCqlMap, decodeBody) {
	{
		cql::CqlMap<cql::CqlText, cql::CqlBigInt> value({ { "abc", 123 } });
		auto data = makeTestString("\x00\x00\x00\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_TRUE(value->empty());
	}
	{
		cql::CqlMap<cql::CqlText, cql::CqlBigInt> value;
		auto data = makeTestString(
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x03""asd"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x64"
			"\x00\x00\x00\x03""qwe"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x65");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value->size(), 2);
		ASSERT_EQ(value->at("asd"), 100);
		ASSERT_EQ(value->at("qwe"), 101);
	}
}

TEST(TestCqlMap, decodeBodyError) {
	{
		cql::CqlMap<cql::CqlText, cql::CqlBigInt> value;
		auto data = makeTestString("\x00\x00\x00");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of map size not enough");
	}
	{
		cql::CqlMap<cql::CqlText, cql::CqlBigInt> value;
		auto data = makeTestString("\xff\xff\xff\xff");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"map size can't be negative");
	}
	{
		cql::CqlMap<cql::CqlText, cql::CqlBigInt> value;
		auto data = makeTestString(
			"\x00\x00\x00\x01"
			"\x00\x00\x00\x03""abc"
			"\x00\x00\x00\x01\xff");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"integer length not matched");
	}
}

TEST(TestCqlMap, operations) {
	{
		// construct with rvalue initializer list
		seastar::sstring keyA("a");
		seastar::sstring keyB("b");
		cql::CqlMap<cql::CqlText, cql::CqlBigInt> value({ { keyA, 1 }, { std::move(keyB), 2 } });
		ASSERT_EQ(keyA, "a");
		ASSERT_EQ(keyB, "");
	}
	{
		// dereference
		cql::CqlMap<cql::CqlText, cql::CqlBigInt> value({ { "abc", 123 } });
		ASSERT_EQ((*value).size(), 1);
		ASSERT_EQ((*value).at("abc"), 123);
	}
	{
		// get pointer
		cql::CqlMap<cql::CqlText, cql::CqlBigInt> value;
		value->emplace("abc", 123);
		ASSERT_EQ(value->size(), 1);
		ASSERT_EQ(value->at("abc"), 123);
	}
}

