#include <CqlDriver/Common/ColumnTypes/CqlUdt.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlInt.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlText.hpp>
#include <TestUtility/GTestUtils.hpp>

namespace {
	struct IdField { using Type = cql::CqlInt; static const constexpr char* Name = "id"; };
	struct NameField { using Type = cql::CqlText; static const constexpr char* Name = "name"; };
	using UdtType = cql::CqlUdt<IdField, NameField>;
}

TEST(TestCqlUdt, fieldInfo) {
	ASSERT_EQ(makeTestString("id"), UdtType::CqlFieldInfo<0>::Name);
	ASSERT_EQ(makeTestString("name"), UdtType::CqlFieldInfo<1>::Name);
	static_assert(std::is_same<UdtType::CqlFieldInfo<0>::Type, cql::CqlInt>::value, "check type");
	static_assert(std::is_same<UdtType::CqlFieldInfo<1>::Type, cql::CqlText>::value, "check type");
}

TEST(TestCqlUdt, getset) {
	UdtType value;
	value.set(std::make_tuple<cql::CqlInt, cql::CqlText>(123, "abc"));
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

TEST(TestCqlUdt, encodeBody) {
	{
		UdtType value(123, "abc");
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc"));
	}
}

TEST(TestCqlUdt, decodeBody) {
	{
		UdtType value;
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""abc");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get<0>(), 123);
		ASSERT_EQ(value.get<1>(), "abc");
	}
	{
		UdtType value(321, "abc");
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get<0>(), 123);
		ASSERT_EQ(value.get<1>(), "");
	}
}

TEST(TestCqlUdt, decodeBodyError) {
	{
		UdtType value;
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of column body size not enough");
	}
	{
		UdtType value;
		auto data = makeTestString(
			"\x00\x00\x00\x04\x00\x00\x00\x7b"
			"\x00\x00\x00\x03""ab");
		ASSERT_THROWS_CONTAINS(
			cql::CqlDecodeException,
			value.decodeBody(data.data(), data.size()),
			"length of column body not enough");
	}
}

TEST(TestCqlUdt, operations) {
	{
		// dereference
		UdtType value(123, "abc");
		ASSERT_EQ(*value, *value);
		ASSERT_EQ(std::get<0>(*value), 123);
		ASSERT_EQ(std::get<1>(*value), "abc");
	}
	{
		// get pointer
		UdtType a(123, "abc");
		UdtType b(321, "asd");
		a->swap(*b);
		ASSERT_EQ(a.get<0>(), 321);
		ASSERT_EQ(a.get<1>(), "asd");
		ASSERT_EQ(b.get<0>(), 123);
		ASSERT_EQ(b.get<1>(), "abc");
	}
}

