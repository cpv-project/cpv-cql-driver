#include <CqlDriver/Utility/StringUtils.hpp>
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

