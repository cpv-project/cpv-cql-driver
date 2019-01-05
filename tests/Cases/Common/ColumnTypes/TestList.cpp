#include <CQLDriver/Common/ColumnTypes/List.hpp>
#include <CQLDriver/Common/ColumnTypes/BigInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestList, getset) {
	cql::List<cql::BigInt> value;
	value.set(std::vector<cql::BigInt>({ 123 }));
	ASSERT_EQ(value.get().size(), 1U);
	ASSERT_EQ(value.get().at(0), 123);

	value.set({ 1, 2, 3 });
	ASSERT_EQ(value.get().size(), 3U);
	ASSERT_EQ(value.get().at(0), 1);
	ASSERT_EQ(value.get().at(1), 2);
	ASSERT_EQ(value.get().at(2), 3);
}

TEST(TestList, encodeBody) {
	{
		cql::List<cql::BigInt> value;
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));
	}
	{
		cql::List<cql::BigInt> value({ 1, 2 });
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x01"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x02"));
	}
}

TEST(TestList, decodeBody) {
	{
		cql::List<cql::BigInt> value({ 1, 2 });
		auto data = makeTestString("\x00\x00\x00\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_TRUE(value->empty());
	}
	{
		cql::List<cql::BigInt> value;
		auto data = makeTestString(
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x01"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x02");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value->size(), 2U);
		ASSERT_EQ(value->at(0), 1);
		ASSERT_EQ(value->at(1), 2);
	}
}

TEST(TestList, decodeBodyError) {
	{
		cql::List<cql::BigInt> value;
		auto data = makeTestString("\x00\x00\x00");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of list size not enough");
	}
	{
		cql::List<cql::BigInt> value;
		auto data = makeTestString("\xff\xff\xff\xff");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"list size can't be negative");
	}
	{
		cql::List<cql::BigInt> value;
		auto data = makeTestString("\x00\x00\x00\x01\x00\x00\x00\x01\xff");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"integer length not matched");
	}
}

TEST(TestList, operations) {
	{
		// dereference
		cql::List<cql::BigInt> value({ 123 });
		ASSERT_EQ((*value).size(), 1U);
		ASSERT_EQ((*value).at(0), 123);
	}
	{
		// get pointer
		cql::List<cql::BigInt> value;
		value->emplace_back(123);
		ASSERT_EQ(value->size(), 1U);
		ASSERT_EQ(value->at(0), 123);
	}
}

