#include <CQLDriver/Common/ColumnTypes/Nullable.hpp>
#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <TestUtility/GTestUtils.hpp>

namespace { using NullableInt = cql::Nullable<cql::Int>; }

TEST(TestNullable, getset) {
	NullableInt value;
	ASSERT_TRUE(value.isNull());
	ASSERT_EQ(value.get(), 0);

	value.set(0);
	ASSERT_FALSE(value.isNull());
	ASSERT_EQ(value.get(), 0);

	value.set(123);
	ASSERT_FALSE(value.isNull());
	ASSERT_EQ(value.get(), 123);

	value = NullableInt(321);
	ASSERT_FALSE(value.isNull());
	ASSERT_EQ(value.get(), 321);
}

TEST(TestNullable, encodeBody) {
	{
		NullableInt value;
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(""));
	}
	{
		NullableInt value(0);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));
	}
	{
		NullableInt value(123);
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x7b"));
	}
}

TEST(TestNullable, decodeBody) {
	{
		NullableInt value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), -1);
		ASSERT_TRUE(value.isNull());
		ASSERT_EQ(value.get(), 0);
	}
	{
		NullableInt value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), 0);
		ASSERT_FALSE(value.isNull());
		ASSERT_EQ(value.get(), 0);
	}
	{
		NullableInt value;
		auto data = makeTestString("\x00\x00\x00\x7b");
		value.decodeBody(data.data(), data.size());
		ASSERT_FALSE(value.isNull());
		ASSERT_EQ(value.get(), 123);
	}
}

TEST(TestNullable, decodeBodyError) {
	NullableInt value;
	auto data = makeTestString("\x00\x00\x7b");
	ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
}

TEST(TestNullable, operations) {
	{
		// dereference
		NullableInt a(123);
		NullableInt b;
		ASSERT_EQ(*a + *b, 123);
		ASSERT_EQ(*a * *b, 0);
		ASSERT_EQ((*b).get(), 0);
	}
	{
		// get pointer
		NullableInt a(123);
		a->reset();
		ASSERT_FALSE(a.isNull());
		ASSERT_EQ(a.get(), 0);
		a->set(321);
		a.reset();
		ASSERT_TRUE(a.isNull());
		ASSERT_EQ(a.get(), 0);
	}
}

