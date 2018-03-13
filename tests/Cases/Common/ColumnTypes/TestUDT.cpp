#include <CQLDriver/Common/ColumnTypes/UDT.hpp>
#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <TestUtility/GTestUtils.hpp>

namespace {
	struct IdField { using Type = cql::Int; static const constexpr char* Name = "id"; };
	struct NameField { using Type = cql::Text; static const constexpr char* Name = "name"; };
	using UDTType = cql::UDT<IdField, NameField>;
}

TEST(TestUDT, fieldInfo) {
	ASSERT_EQ(makeTestString("id"), UDTType::FieldInfo<0>::Name);
	ASSERT_EQ(makeTestString("name"), UDTType::FieldInfo<1>::Name);
	static_assert(std::is_same<UDTType::FieldInfo<0>::Type, cql::Int>::value, "check type");
	static_assert(std::is_same<UDTType::FieldInfo<1>::Type, cql::Text>::value, "check type");
}

TEST(TestUDT, getset) {
	UDTType value;
	value.set(std::make_tuple<cql::Int, cql::Text>(123, "abc"));
	ASSERT_EQ(std::get<0>(value.get()), 123);
	ASSERT_EQ(std::get<1>(value.get()), "abc");

	value.set(321, "asd");
	ASSERT_EQ(std::get<0>(value.get()), 321);
	ASSERT_EQ(std::get<1>(value.get()), "asd");

	value.set<0>(128);
	value.set<1>("qwe");
	ASSERT_EQ(value.get<0>(), 128);
	ASSERT_EQ(value.get<1>(), "qwe");;
}

TEST(TestUDT, encodeBody) {
	{
		UDTType value(123, "abc");
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc"));
	}
}

TEST(TestUDT, decodeBody) {
	{
		UDTType value;
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get<0>(), 123);
		ASSERT_EQ(value.get<1>(), "abc");
	}
	{
		UDTType value(321, "abc");
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get<0>(), 123);
		ASSERT_EQ(value.get<1>(), "");
	}
}

TEST(TestUDT, decodeBodyError) {
	{
		UDTType value;
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of column body size not enough");
	}
	{
		UDTType value;
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""ab");
		ASSERT_THROWS_CONTAINS(
			cql::DecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of column body not enough");
	}
}

TEST(TestUDT, operations) {
	{
		// dereference
		UDTType value(123, "abc");
		ASSERT_EQ(*value, *value);
		ASSERT_EQ(std::get<0>(*value), 123);
		ASSERT_EQ(std::get<1>(*value), "abc");
	}
	{
		// get pointer
		UDTType a(123, "abc");
		UDTType b(321, "asd");
		a->swap(*b);
		ASSERT_EQ(a.get<0>(), 321);
		ASSERT_EQ(a.get<1>(), "asd");
		ASSERT_EQ(b.get<0>(), 123);
		ASSERT_EQ(b.get<1>(), "abc");
	}
}

