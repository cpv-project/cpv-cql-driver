#include <CQLDriver/Common/ColumnTrait.hpp>
#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <CQLDriver/Common/ColumnTypes/Nullable.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestColumnTrait, underlyingType) {
	static_assert(
		std::is_same<
			cql::ColumnTrait<cql::Int>::UnderlyingType,
			cql::Int::UnderlyingType>::value,
		"test underlying type");
}

TEST(TestColumnTrait, isNullable) {
	static_assert(
		!cql::ColumnTrait<cql::Int>::IsNullable::value,
		"test is not nullable");
	static_assert(
		cql::ColumnTrait<cql::Nullable<cql::Int>>::IsNullable::value,
		"test is nullable");
}

TEST(TestColumnTrait, hash) {
	cql::Int valueA(123);
	cql::Int valueB(123);
	auto hashA = cql::ColumnTrait<cql::Int>::Hash()(valueA);
	auto hashB = cql::ColumnTrait<cql::Int>::Hash()(valueB);
	ASSERT_EQ(hashA, hashB);
}

TEST(TestColumnTrait, encodeInt) {
	{
		cql::Int value;
		seastar::sstring data;
		cql::ColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x04\x00\x00\x00\x00"));
	}
	{
		cql::Int value(123);
		seastar::sstring data;
		cql::ColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x04\x00\x00\x00\x7b"));
	}
}

TEST(TestColumnTrait, decodeInt) {
	{
		cql::Int value(123);
		auto data = makeTestString("\x00\x00\x00\x00");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::ColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_EQ(value, 0);
	}
	{
		cql::Int value;
		auto data = makeTestString("\x00\x00\x00\x04\x00\x00\x00\xff");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::ColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_EQ(value, 255);
	}
}

TEST(TestColumnTrait, decodeIntError) {
	{
		cql::Int value;
		auto data = makeTestString("\x00\x00\x00");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::DecodeException,
			cql::ColumnTrait<decltype(value)>::decode(value, ptr, end),
			"length of column body size not enough");
	}
	{
		cql::Int value;
		auto data = makeTestString("\x00\x00\x00\x04\x00\x00\x00");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::DecodeException,
			cql::ColumnTrait<decltype(value)>::decode(value, ptr, end),
			"length of column body not enough");
	}
	{
		cql::Int value;
		auto data = makeTestString("\x00\x00\x00\x05\x00\x00\x00\x00\x01");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::DecodeException,
			cql::ColumnTrait<decltype(value)>::decode(value, ptr, end),
			"integer length not matched");
	}
	{
		cql::Int value;
		auto data = makeTestString("\xff\xff\xff\xff");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::DecodeException,
			cql::ColumnTrait<decltype(value)>::decode(value, ptr, end),
			"column is not nullable but the value is null");
	}
}

TEST(TestColumnTrait, encodeText) {
	{
		cql::Text value;
		seastar::sstring data;
		cql::ColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00"));
	}
	{
		cql::Text value("abc");
		seastar::sstring data;
		cql::ColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x03""abc"));
	}
}

TEST(TestColumnTrait, decodeText) {
	{
		cql::Text value;
		auto data = makeTestString("\x00\x00\x00\x00");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::ColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_EQ(value, "");
	}
	{
		cql::Text value;
		auto data = makeTestString("\x00\x00\x00\x03""abc");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::ColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_EQ(value, "abc");
	}
}

TEST(TestColumnTrait, decodeTextError) {
	{
		cql::Text value;
		auto data = makeTestString("\x00\x00\x00");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::DecodeException,
			cql::ColumnTrait<decltype(value)>::decode(value, ptr, end),
			"length of column body size not enough");
	}
	{
		cql::Text value;
		auto data = makeTestString("\x00\x00\x00\x04""abc");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::DecodeException,
			cql::ColumnTrait<decltype(value)>::decode(value, ptr, end),
			"length of column body not enough");
	}
	{
		cql::Text value;
		auto data = makeTestString("\xff\xff\xff\xff");
		const char* ptr = data.begin();
		const char* end = data.end();
		ASSERT_THROWS_CONTAINS(cql::DecodeException,
			cql::ColumnTrait<decltype(value)>::decode(value, ptr, end),
			"column is not nullable but the value is null");
	}
}

TEST(TestColumnTrait, encodeNullableInt) {
	{
		cql::Nullable<cql::Int> value;
		seastar::sstring data;
		cql::ColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\xff\xff\xff\xff"));
	}
	{
		cql::Nullable<cql::Int> value(0);
		seastar::sstring data;
		cql::ColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x04\x00\x00\x00\x00"));
	}
	{
		cql::Nullable<cql::Int> value(123);
		seastar::sstring data;
		cql::ColumnTrait<decltype(value)>::encode(value, data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x04\x00\x00\x00\x7b"));
	}
}

TEST(TestColumnTrait, decodeNullableInt) {
	{
		cql::Nullable<cql::Int> value(123);
		auto data = makeTestString("\xff\xff\xff\xff");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::ColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_TRUE(value.isNull());
		ASSERT_EQ(*value, 0);
	}
	{
		cql::Nullable<cql::Int> value(123);
		auto data = makeTestString("\x00\x00\x00\x00");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::ColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_FALSE(value.isNull());
		ASSERT_EQ(*value, 0);
	}
	{
		cql::Nullable<cql::Int> value;
		auto data = makeTestString("\x00\x00\x00\x04\x00\x00\x00\xff");
		const char* ptr = data.begin();
		const char* end = data.end();
		cql::ColumnTrait<decltype(value)>::decode(value, ptr, end);
		ASSERT_FALSE(value.isNull());
		ASSERT_EQ(*value, 255);
	}
}

