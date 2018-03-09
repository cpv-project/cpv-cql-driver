#include <CQLDriver/Common/Utility/StringUtils.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestStringUtils, split) {
	std::vector<std::string> results;
	std::size_t countRecord = 0;
	std::string str("aaa \tb  c\nd eee");
	cql::splitString(str,
		[&results, &countRecord, &str](auto startIndex, auto endIndex, auto count) {
			ASSERT_EQ(countRecord, count);
			++countRecord;
			results.emplace_back(str, startIndex, endIndex - startIndex);
		});
	ASSERT_EQ(results.size(), 5);
	ASSERT_EQ(results.at(0), "aaa");
	ASSERT_EQ(results.at(1), "b");
	ASSERT_EQ(results.at(2), "c");
	ASSERT_EQ(results.at(3), "d");
	ASSERT_EQ(results.at(4), "eee");
}

TEST(TestStringUtils, join) {
	std::string a = cql::joinString(" ", "a", 123, "asd");
	ASSERT_EQ(a, "a 123 asd");
	std::string b = cql::joinString("-", "b");
	ASSERT_EQ(b, "b");
	std::string c = cql::joinString("", 1, 2, 888);
	ASSERT_EQ(c, "12888");
}

TEST(TestStringUtils, dumpIntToHex) {
	{
		std::string str;
		cql::dumpIntToHex<std::uint8_t>(0x7f, str);
		ASSERT_EQ(str, "7F");
	}
	{
		std::string str;
		cql::dumpIntToHex<std::int32_t>(0x12345678, str);
		ASSERT_EQ(str, "12345678");
	}
	{
		std::string str;
		cql::dumpIntToHex<std::int32_t>(-0x12345678, str);
		ASSERT_EQ(str, "EDCBA988");
	}
}

TEST(TestStringUtils, dumpBytesToHex) {
	{
		std::string str;
		cql::dumpBytesToHex("abc", 3, str);
		ASSERT_EQ(str, "616263");
	}
	{
		std::string str;
		cql::dumpBytesToHex("", 0, str);
		ASSERT_EQ(str, "");
	}
}

TEST(TestStringUtils, loadIntFromHex) {
	{
		std::uint8_t value = 0;
		ASSERT_TRUE(cql::loadIntFromHex("7F", value));
		ASSERT_EQ(value, 0x7f);
	}
	{
		std::uint8_t value = 0;
		ASSERT_TRUE(cql::loadIntFromHex("7f", value));
		ASSERT_EQ(value, 0x7f);
	}
	{
		std::int32_t value = 0;
		ASSERT_TRUE(cql::loadIntFromHex("12345678", value));
		ASSERT_EQ(value, 0x12345678);
	}
	{
		std::int32_t value = 0;
		ASSERT_TRUE(cql::loadIntFromHex("EDCBA988", value));
		ASSERT_EQ(value, -0x12345678);
	}
	{
		std::int32_t value = 0;
		ASSERT_TRUE(cql::loadIntFromHex("EDCBa988", value));
		ASSERT_EQ(value, -0x12345678);
	}
	{
		std::int32_t value = 0;
		ASSERT_FALSE(cql::loadIntFromHex("EDCBag88", value));
	}
	{
		std::int32_t value = 0;
		ASSERT_FALSE(cql::loadIntFromHex("EDCBa98", value));
	}
}

TEST(TestStringUtils, loadBytesFromHex) {
	{
		std::string str;
		ASSERT_TRUE(cql::loadBytesFromHex("616263", 6, str));
		ASSERT_EQ(str, "abc");
	}
	{
		std::string str;
		ASSERT_TRUE(cql::loadBytesFromHex("aa7F001f", 8, str));
		ASSERT_EQ(str, std::string("\xaa\x7f\x00\x1f", 4));
	}
	{
		std::string str;
		ASSERT_FALSE(cql::loadBytesFromHex("aa7F001g", 8, str));
	}
	{
		std::string str;
		ASSERT_FALSE(cql::loadBytesFromHex("aa7F00", 8, str));
	}
}

