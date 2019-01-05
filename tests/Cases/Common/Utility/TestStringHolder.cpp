#include <unordered_map>
#include <CQLDriver/Common/Utility/StringHolder.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestStringHolder, hasOwnership) {
	cql::StringHolder a(std::string("test abc"));
	cql::StringHolder b("abc", 3);
	cql::StringHolder c("qwert");
	ASSERT_TRUE(a.hasOwnership());
	ASSERT_FALSE(b.hasOwnership());
	ASSERT_FALSE(c.hasOwnership());
}

TEST(TestStringHolder, get) {
	cql::StringHolder a(std::string("test abc"));
	cql::StringHolder b("abc", 3);
	cql::StringHolder c("qwert");
	ASSERT_EQ(a.get(), "test abc");
	ASSERT_EQ(b.get(), "abc");
	ASSERT_EQ(c.get(), "qwert");
}

TEST(TestStringHolder, compare) {
	cql::StringHolder a(std::string("abc"));
	cql::StringHolder b("abc", 3);
	cql::StringHolder c("qwert");
	ASSERT_TRUE(a == b);
	ASSERT_TRUE(b == a);
	ASSERT_FALSE(a == c);
	ASSERT_FALSE(b == c);
	ASSERT_FALSE(a != b);
	ASSERT_FALSE(b != a);
	ASSERT_TRUE(a != c);
	ASSERT_TRUE(b != c);
}

TEST(TestStringHolder, asMapKey) {
	std::unordered_map<cql::StringHolder, std::size_t, cql::StringHolder::Hash> map;
	map.emplace("abc", 123);
	map.emplace("asd", 100);
	ASSERT_EQ(map.at(cql::StringHolder("abc")), 123U);
	ASSERT_EQ(map.at(cql::StringHolder("asd")), 100U);
	cql::StringHolder::Hash hash;
	ASSERT_TRUE(hash(cql::StringHolder("a")) != hash(cql::StringHolder("b")));
}

