#include <CqlDriver/Common/ColumnTypes/CqlTuple.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlInt.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlText.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlTuple, getset) {
	cql::CqlTuple<cql::CqlInt, cql::CqlText> value;
	value.set(std::make_tuple<cql::CqlInt, cql::CqlText>(123, "abc"));
	ASSERT_EQ(std::get<0>(value.get()), 123);
	ASSERT_EQ(std::get<1>(value.get()), "abc");

	value.set(321, "asd");
	ASSERT_EQ(std::get<0>(value.get()), 321);
	ASSERT_EQ(std::get<1>(value.get()), "asd");

	value.set<0>(128);
	value.set<1>("qwe");
	ASSERT_EQ(value.get<0>(), 128);
	ASSERT_EQ(value.get<1>(), "qwe");
}

TEST(TestCqlTuple, encodeBody) {
	{
		cql::CqlTuple<> value;
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(""));
	}
	{
		cql::CqlTuple<cql::CqlInt, cql::CqlText> value(123, "abc");
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc"));
	}
}

TEST(TestCqlTuple, decodeBody) {
	{
		cql::CqlTuple<> value;
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
	}
	{
		cql::CqlTuple<cql::CqlInt, cql::CqlText> value;
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get<0>(), 123);
		ASSERT_EQ(value.get<1>(), "abc");
	}
}

TEST(TestCqlTuple, decodeBodyError) {
	{
		cql::CqlTuple<cql::CqlInt, cql::CqlText> value;
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of column body size not enough");
	}
	{
		cql::CqlTuple<cql::CqlInt, cql::CqlText> value;
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""ab");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of column body not enough");
	}
}

TEST(TestCqlTuple, operations) {
	{
		// dereference
		cql::CqlTuple<cql::CqlInt, cql::CqlText> value(123, "abc");
		ASSERT_EQ(*value, *value);
		ASSERT_EQ(std::get<0>(*value), 123);
		ASSERT_EQ(std::get<1>(*value), "abc");
	}
	{
		// get pointer
		cql::CqlTuple<cql::CqlInt, cql::CqlText> a(123, "abc");
		cql::CqlTuple<cql::CqlInt, cql::CqlText> b(321, "asd");
		a->swap(*b);
		ASSERT_EQ(a.get<0>(), 321);
		ASSERT_EQ(a.get<1>(), "asd");
		ASSERT_EQ(b.get<0>(), 123);
		ASSERT_EQ(b.get<1>(), "abc");
	}
}

