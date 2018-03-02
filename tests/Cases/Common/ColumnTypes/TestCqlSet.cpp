#include <CqlDriver/Common/ColumnTypes/CqlSet.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlBigInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlSet, getset) {
	using SetType = std::unordered_set<cql::CqlBigInt, cql::CqlColumnTrait<cql::CqlBigInt>::Hash>;
	cql::CqlSet<cql::CqlBigInt> value;
	value.set(SetType({ cql::CqlBigInt(123) }));
	ASSERT_EQ(value.get().size(), 1);
	ASSERT_EQ(value.get().count(cql::CqlBigInt(123)), 1);

	value.set({ 1, 2, 3, 2, 3 });
	ASSERT_EQ(value.get().size(), 3);
	ASSERT_EQ(value.get().count(cql::CqlBigInt(1)), 1);
	ASSERT_EQ(value.get().count(cql::CqlBigInt(2)), 1);
	ASSERT_EQ(value.get().count(cql::CqlBigInt(3)), 1);
}

TEST(TestCqlSet, encodeBody) {
	{
		cql::CqlSet<cql::CqlBigInt> value;
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));
	}
	{
		cql::CqlSet<cql::CqlBigInt> value({ 1 });
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x01"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x01"));
	}
}

TEST(TestCqlSet, decodeBody) {
	{
		cql::CqlSet<cql::CqlBigInt> value({ 1, 2 });
		auto data = makeTestString("\x00\x00\x00\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_TRUE(value->empty());
	}
	{
		cql::CqlSet<cql::CqlBigInt> value;
		auto data = makeTestString(
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x01"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x02");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value->size(), 2);
		ASSERT_EQ(value->count(cql::CqlBigInt(1)), 1);
		ASSERT_EQ(value->count(cql::CqlBigInt(2)), 1);
	}
}

TEST(TestCqlSet, decodeBodyError) {
	{
		cql::CqlSet<cql::CqlBigInt> value;
		auto data = makeTestString("\x00\x00\x00");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of set size not enough");
	}
	{
		cql::CqlSet<cql::CqlBigInt> value;
		auto data = makeTestString("\xff\xff\xff\xff");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"set size can't be negative");
	}
	{
		cql::CqlSet<cql::CqlBigInt> value;
		auto data = makeTestString("\x00\x00\x00\x01\x00\x00\x00\x01\xff");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"integer length not matched");
	}
}

TEST(TestCqlSet, operations) {
	{
		// dereference
		cql::CqlSet<cql::CqlBigInt> value({ 123 });
		ASSERT_EQ((*value).size(), 1);
		ASSERT_EQ((*value).count(cql::CqlBigInt(123)), 1);
	}
	{
		// get pointer
		cql::CqlSet<cql::CqlBigInt> value;
		value->emplace(123);
		ASSERT_EQ(value->size(), 1);
		ASSERT_EQ(value->count(cql::CqlBigInt(123)), 1);
	}
}

