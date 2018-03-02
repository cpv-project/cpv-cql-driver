#include <CqlDriver/Common/CqlColumnTrait.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlInt.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlText.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlNullable.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlColumnTrait, underlyingType) {
	static_assert(
		std::is_same<
			cql::CqlColumnTrait<cql::CqlInt>::CqlUnderlyingType,
			cql::CqlInt::CqlUnderlyingType>::value,
		"test underlying type");
}

TEST(TestCqlColumnTrait, isNullable) {
	static_assert(
		!cql::CqlColumnTrait<cql::CqlInt>::CqlIsNullable::value,
		"test is not nullable");
	static_assert(
		cql::CqlColumnTrait<cql::CqlNullable<cql::CqlInt>>::CqlIsNullable::value,
		"test is nullable");
}

TEST(TestCqlColumnTrait, hash) {
	cql::CqlInt valueA(123);
	cql::CqlInt valueB(123);
	auto hashA = cql::CqlColumnTrait<cql::CqlInt>::Hash()(valueA);
	auto hashB = cql::CqlColumnTrait<cql::CqlInt>::Hash()(valueB);
	ASSERT_EQ(hashA, hashB);
}

TEST(TestCqlColumnTrait, encodeInt) {
	{
		cql::CqlInt value;
		seastar::sstring data;
		cql::CqlColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x04\x00\x00\x00\x00"));
	}
	{
		cql::CqlInt value(123);
		seastar::sstring data;
		cql::CqlColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x04\x00\x00\x00\x7b"));
	}
}

TEST(TestCqlColumnTrait, decodeInt) {
	{
		cql::CqlInt value(123);
		auto data = makeTestString("\x00\x00\x00\x00");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_EQ(value, 0);
	}
	{
		cql::CqlInt value;
		auto data = makeTestString("\x00\x00\x00\x04\x00\x00\x00\xff");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_EQ(value, 255);
	}
}

TEST(TestCqlColumnTrait, decodeIntError) {
	{
		cql::CqlInt value;
		auto data = makeTestString("\x00\x00\x00");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::CqlDecodeException,
			cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end),
			"length of column body size not enough");
	}
	{
		cql::CqlInt value;
		auto data = makeTestString("\x00\x00\x00\x04\x00\x00\x00");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::CqlDecodeException,
			cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end),
			"length of column body not enough");
	}
	{
		cql::CqlInt value;
		auto data = makeTestString("\x00\x00\x00\x05\x00\x00\x00\x00\x01");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::CqlDecodeException,
			cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end),
			"integer length not matched");
	}
	{
		cql::CqlInt value;
		auto data = makeTestString("\xff\xff\xff\xff");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::CqlDecodeException,
			cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end),
			"column is not nullable but the value is null");
	}
}

TEST(TestCqlColumnTrait, encodeText) {
	{
		cql::CqlText value;
		seastar::sstring data;
		cql::CqlColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));
	}
	{
		cql::CqlText value("abc");
		seastar::sstring data;
		cql::CqlColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x03""abc"));
	}
}

TEST(TestCqlColumnTrait, decodeText) {
	{
		cql::CqlText value;
		auto data = makeTestString("\x00\x00\x00\x00");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_EQ(value, "");
	}
	{
		cql::CqlText value;
		auto data = makeTestString("\x00\x00\x00\x03""abc");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_EQ(value, "abc");
	}
}

TEST(TestCqlColumnTrait, decodeTextError) {
	{
		cql::CqlText value;
		auto data = makeTestString("\x00\x00\x00");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::CqlDecodeException,
			cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end),
			"length of column body size not enough");
	}
	{
		cql::CqlText value;
		auto data = makeTestString("\x00\x00\x00\x04""abc");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::CqlDecodeException,
			cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end),
			"length of column body not enough");
	}
	{
		cql::CqlText value;
		auto data = makeTestString("\xff\xff\xff\xff");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::CqlDecodeException,
			cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end),
			"column is not nullable but the value is null");
	}
}

TEST(TestCqlColumnTrait, encodeNullableInt) {
	{
		cql::CqlNullable<cql::CqlInt> value;
		seastar::sstring data;
		cql::CqlColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\xff\xff\xff\xff"));
	}
	{
		cql::CqlNullable<cql::CqlInt> value(0);
		seastar::sstring data;
		cql::CqlColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x04\x00\x00\x00\x00"));
	}
	{
		cql::CqlNullable<cql::CqlInt> value(123);
		seastar::sstring data;
		cql::CqlColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x04\x00\x00\x00\x7b"));
	}
}

TEST(TestCqlColumnTrait, decodeNullableInt) {
	{
		cql::CqlNullable<cql::CqlInt> value(123);
		auto data = makeTestString("\xff\xff\xff\xff");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_TRUE(value.isNull());
		ASSERT_EQ(*value, 0);
	}
	{
		cql::CqlNullable<cql::CqlInt> value(123);
		auto data = makeTestString("\x00\x00\x00\x00");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_FALSE(value.isNull());
		ASSERT_EQ(*value, 0);
	}
	{
		cql::CqlNullable<cql::CqlInt> value;
		auto data = makeTestString("\x00\x00\x00\x04\x00\x00\x00\xff");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::CqlColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_FALSE(value.isNull());
		ASSERT_EQ(*value, 255);
	}
}

