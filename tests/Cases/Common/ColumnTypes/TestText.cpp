#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <CQLDriver/Common/Utility/StringUtils.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestText, getset) {
	cql::Text value("abc");
	ASSERT_EQ(value.get(), "abc");
	value.get().append("aaaaa", 5);
	ASSERT_EQ(value.get(), "abcaaaaa");

	value.set("abc\x00\x01");
	ASSERT_EQ(value.get(), makeTestString("abc\x00\x01"));

	value.set("abcde", 3);
	ASSERT_EQ(value.get(), "abc");

	value = cql::Text();
	ASSERT_EQ(value.get(), "");

	value = cql::Text("qwert");
	ASSERT_EQ(value.get(), "qwert");
	ASSERT_EQ(value.get(), std::string(value.data(), value.size()));
	ASSERT_FALSE(value.empty());
	value.reset();
	ASSERT_EQ(value.get(), "");
	ASSERT_TRUE(value.empty());
}

TEST(TestText, encodeBody) {
	cql::Text value("abc\x00\x01");
	std::string data;
	value.encodeBody(data);
	ASSERT_EQ(data, makeTestString("abc\x00\x01"));
}

TEST(TestText, decodeBody) {
	auto data = makeTestString("abc\x00\x01");
	cql::Text value;
	value.decodeBody(data.data(), data.size());
	ASSERT_TRUE(value == makeTestString("abc\x00\x01"));
}

TEST(TestText, operations) {
	{
		// assign and cast
		cql::Text value;
		value = std::string("aaa");
		std::string str = value;
		ASSERT_EQ(str, "aaa");
	}
	{
		// cast (contains \x00)
		cql::Text value("abc\x00\x01");
		std::string str = value;
		ASSERT_EQ(str, makeTestString("abc\x00\x01"));
	}
	{
		// dereference
		cql::Text value("abc");
		ASSERT_EQ(*value, "abc");
	}
	{
		// get pointer
		cql::Text value("abc");
		value->append("de", 2);
		ASSERT_EQ(value->size(), 5U);
	}
	{
		// equal to
		cql::Text value("abc");
		ASSERT_TRUE(value == std::string("abc"));
		ASSERT_FALSE(value == std::string("cba"));
		ASSERT_TRUE(value == "abc");
		ASSERT_FALSE(value == "cba");
		ASSERT_TRUE(std::string("abc") == value);
		ASSERT_FALSE(std::string("cba") == value);
		ASSERT_TRUE("abc" == value);
		ASSERT_FALSE("cba" == value);
	}
	{
		// not equal to
		cql::Text value("abc");
		ASSERT_TRUE(value != std::string("cba"));
		ASSERT_FALSE(value != std::string("abc"));
		ASSERT_TRUE(value != "cba");
		ASSERT_FALSE(value != "abc");
		ASSERT_TRUE(std::string("cba") != value);
		ASSERT_FALSE(std::string("abc") != value);
		ASSERT_TRUE("cba" != value);
		ASSERT_FALSE("abc" != value);
	}
	{
		// get text description
		cql::Text value("qwert");
		ASSERT_EQ(cql::joinString("", value), "qwert");
	}
}

