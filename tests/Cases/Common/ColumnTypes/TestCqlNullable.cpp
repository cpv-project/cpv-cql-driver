#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlNullable.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlInt.hpp>
#include <TestUtility/GTestUtils.hpp>

namespace { using NullableInt = cql::CqlNullable<cql::CqlInt>; }

TEST(TestCqlNullable, getset) {
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

TEST(TestCqlNullable, encode) {
	{
		NullableInt value;
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(""));
	}
	{
		NullableInt value(0);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));
	}
	{
		NullableInt value(123);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x7b"));
	}
}

TEST(TestCqlNullable, decode) {
	{
		NullableInt value(123);
		auto data = makeTestString("");
		value.decode(data.data(), -1);
		ASSERT_TRUE(value.isNull());
		ASSERT_EQ(value.get(), 0);
	}
	{
		NullableInt value(123);
		auto data = makeTestString("");
		value.decode(data.data(), 0);
		ASSERT_FALSE(value.isNull());
		ASSERT_EQ(value.get(), 0);
	}
	{
		NullableInt value;
		auto data = makeTestString("\x00\x00\x00\x7b");
		value.decode(data.data(), data.size());
		ASSERT_FALSE(value.isNull());
		ASSERT_EQ(value.get(), 123);
	}
}

TEST(TestCqlNullable, decodeError) {
	NullableInt value;
	auto data = makeTestString("\x00\x00\x7b");
	ASSERT_THROWS(cql::CqlDecodeException, value.decode(data.data(), data.size()));
}

TEST(TestCqlNullable, operations) {
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

