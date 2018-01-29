#include <CqlDriver/Common/ColumnTypes/CqlBlob.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlBlob, getset) {
	cql::CqlBlob value("abc");
	ASSERT_EQ(value.get(), "abc");
	value.get().append("aaaaa", 5);
	ASSERT_EQ(value.get(), "abcaaaaa");

	value.set("abc\x00\x01");
	ASSERT_EQ(value.get(), makeTestString("abc\x00\x01"));

	value.set("abcde", 3);
	ASSERT_EQ(value.get(), "abc");

	value = cql::CqlBlob();
	ASSERT_EQ(value.get(), "");

	value = cql::CqlBlob("qwert");
	ASSERT_EQ(value.get(), "qwert");
	ASSERT_EQ(value.get(), seastar::sstring(value.data(), value.size()));
	ASSERT_FALSE(value.empty());
	value.reset();
	ASSERT_EQ(value.get(), "");
	ASSERT_TRUE(value.empty());
}

TEST(TestCqlBlob, encode) {
	cql::CqlBlob value("abc\x00\x01");
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("abc\x00\x01"));
}

TEST(TestCqlBlob, decode) {
	auto data = makeTestString("abc\x00\x01");
	cql::CqlBlob value;
	value.decode(data.data(), data.size());
	ASSERT_TRUE(value == makeTestString("abc\x00\x01"));
}

TEST(TestCqlBlob, operations) {
	cql::CqlBlob value;
	value = seastar::sstring("aaa");

	seastar::sstring str = value;
	ASSERT_EQ(str, "aaa");

	value = "abc\x00\x01";
	str = value;
	ASSERT_EQ(str, makeTestString("abc\x00\x01"));

	value = "abc";
	ASSERT_TRUE(value == seastar::sstring("abc"));
	ASSERT_FALSE(value == seastar::sstring("cba"));
	ASSERT_TRUE(value != seastar::sstring("cba"));
	ASSERT_FALSE(value != seastar::sstring("abc"));
	ASSERT_TRUE(value == "abc");
	ASSERT_FALSE(value == "cba");
	ASSERT_TRUE(value != "cba");
	ASSERT_FALSE(value != "abc");
}

