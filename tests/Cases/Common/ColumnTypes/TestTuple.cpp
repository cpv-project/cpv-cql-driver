#include <CQLDriver/Common/ColumnTypes/Tuple.hpp>
#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestTuple, getset) {
	cql::Tuple<cql::Int, cql::Text> value;
	value.set(std::make_tuple<cql::Int, cql::Text>(123, "abc"));
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

TEST(TestTuple, encodeBody) {
	{
		cql::Tuple<> value;
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(""));
	}
	{
		cql::Tuple<cql::Int, cql::Text> value(123, "abc");
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc"));
	}
}

TEST(TestTuple, decodeBody) {
	{
		cql::Tuple<> value;
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
	}
	{
		cql::Tuple<cql::Int, cql::Text> value;
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get<0>(), 123);
		ASSERT_EQ(value.get<1>(), "abc");
	}
}

TEST(TestTuple, decodeBodyError) {
	{
		cql::Tuple<cql::Int, cql::Text> value;
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of column body size not enough");
	}
	{
		cql::Tuple<cql::Int, cql::Text> value;
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""ab");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of column body not enough");
	}
}

TEST(TestTuple, operations) {
	{
		// dereference
		cql::Tuple<cql::Int, cql::Text> value(123, "abc");
		ASSERT_EQ(*value, *value);
		ASSERT_EQ(std::get<0>(*value), 123);
		ASSERT_EQ(std::get<1>(*value), "abc");
	}
	{
		// get pointer
		cql::Tuple<cql::Int, cql::Text> a(123, "abc");
		cql::Tuple<cql::Int, cql::Text> b(321, "asd");
		a->swap(*b);
		ASSERT_EQ(a.get<0>(), 321);
		ASSERT_EQ(a.get<1>(), "asd");
		ASSERT_EQ(b.get<0>(), 123);
		ASSERT_EQ(b.get<1>(), "abc");
	}
}

