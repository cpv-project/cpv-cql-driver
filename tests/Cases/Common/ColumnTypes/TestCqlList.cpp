#include <CqlDriver/Common/ColumnTypes/CqlList.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlBigInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlList, getset) {
	cql::CqlList<cql::CqlBigInt> value;
	value.set(std::vector<cql::CqlBigInt>({ 123 }));
	ASSERT_EQ(value.get().size(), 1);
	ASSERT_EQ(value.get().at(0), 123);

	value.set({ 1, 2, 3 });
	ASSERT_EQ(value.get().size(), 3);
	ASSERT_EQ(value.get().at(0), 1);
	ASSERT_EQ(value.get().at(1), 2);
	ASSERT_EQ(value.get().at(2), 3);
}

TEST(TestCqlList, encodeBody) {
	{
		cql::CqlList<cql::CqlBigInt> value;
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));
	}
	{
		cql::CqlList<cql::CqlBigInt> value({ 1, 2 });
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x01"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x02"));
	}
}

TEST(TestCqlList, decodeBody) {
	{
		cql::CqlList<cql::CqlBigInt> value({ 1, 2 });
		auto data = makeTestString("\x00\x00\x00\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_TRUE(value->empty());
	}
	{
		cql::CqlList<cql::CqlBigInt> value;
		auto data = makeTestString(
			"\x00\x00\x00\x02"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x01"
			"\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00\x02");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value->size(), 2);
		ASSERT_EQ(value->at(0), 1);
		ASSERT_EQ(value->at(1), 2);
	}
}

TEST(TestCqlList, decodeBodyError) {
	{
		cql::CqlList<cql::CqlBigInt> value;
		auto data = makeTestString("\x00\x00\x00");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of list size not enough");
	}
	{
		cql::CqlList<cql::CqlBigInt> value;
		auto data = makeTestString("\xff\xff\xff\xff");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"list size can't be negative");
	}
	{
		cql::CqlList<cql::CqlBigInt> value;
		auto data = makeTestString("\x00\x00\x00\x01\x00\x00\x00\x01\xff");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"integer length not matched");
	}
}

TEST(TestCqlList, operations) {
	{
		// dereference
		cql::CqlList<cql::CqlBigInt> value({ 123 });
		ASSERT_EQ((*value).size(), 1);
		ASSERT_EQ((*value).at(0), 123);
	}
	{
		// get pointer
		cql::CqlList<cql::CqlBigInt> value;
		value->emplace_back(123);
		ASSERT_EQ(value->size(), 1);
		ASSERT_EQ(value->at(0), 123);
	}
}

