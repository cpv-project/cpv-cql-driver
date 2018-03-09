#include <CQLDriver/Common/ColumnTypes/Set.hpp>
#include <CQLDriver/Common/ColumnTypes/BigInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestSet, getset) {
	using SetType = std::unordered_set<cql::BigInt, cql::ColumnTrait<cql::BigInt>::Hash>;
	cql::Set<cql::BigInt> value;
	value.set(SetType({ 123 }));
	ASSERT_EQ(value.get().size(), 1);
	ASSERT_EQ(value.get().count(123), 1);

	value.set({ 1, 2, 3, 2, 3 });
	ASSERT_EQ(value.get().size(), 3);
	ASSERT_EQ(value.get().count(1), 1);
	ASSERT_EQ(value.get().count(2), 1);
	ASSERT_EQ(value.get().count(3), 1);
}

TEST(TestSet, encodeBody) {
	{
		cql::Set<cql::BigInt> value;
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));
	}
	{
		cql::Set<cql::BigInt> value({ 1 });
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x01"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x01"));
	}
}

TEST(TestSet, decodeBody) {
	{
		cql::Set<cql::BigInt> value({ 1, 2 });
		auto data = makeTestString("\x00\x00\x00\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_TRUE(value->empty());
	}
	{
		cql::Set<cql::BigInt> value;
		auto data = makeTestString(
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x01"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x02");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value->size(), 2);
		ASSERT_EQ(value->count(1), 1);
		ASSERT_EQ(value->count(2), 1);
	}
}

TEST(TestSet, decodeBodyError) {
	{
		cql::Set<cql::BigInt> value;
		auto data = makeTestString("\x00\x00\x00");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of set size not enough");
	}
	{
		cql::Set<cql::BigInt> value;
		auto data = makeTestString("\xff\xff\xff\xff");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"set size can't be negative");
	}
	{
		cql::Set<cql::BigInt> value;
		auto data = makeTestString("\x00\x00\x00\x01\x00\x00\x00\x01\xff");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"integer length not matched");
	}
}

TEST(TestSet, operations) {
	{
		// dereference
		cql::Set<cql::BigInt> value({ 123 });
		ASSERT_EQ((*value).size(), 1);
		ASSERT_EQ((*value).count(123), 1);
	}
	{
		// get pointer
		cql::Set<cql::BigInt> value;
		value->emplace(123);
		ASSERT_EQ(value->size(), 1);
		ASSERT_EQ(value->count(123), 1);
	}
}

