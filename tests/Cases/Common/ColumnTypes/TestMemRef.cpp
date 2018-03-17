#include <CQLDriver/Common/ColumnTypes/MemRef.hpp>
#include <CQLDriver/Common/Utility/StringUtils.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestMemRef, getset) {
	cql::MemRef value("abc");
	ASSERT_EQ(std::string(value.get().first, value.get().second), "abc");

	value.set("abc\x00\x01");
	ASSERT_EQ(std::string(value.data(), value.size()), makeTestString("abc\x00\x01"));

	value = cql::MemRef();
	ASSERT_EQ(value.get().first, nullptr);
	ASSERT_EQ(value.get().second, 0);

	value = cql::MemRef("qwert", 3);
	ASSERT_EQ(std::string(value.data(), value.size()), makeTestString("qwe"));
	ASSERT_FALSE(value.empty());
	value.reset();
	ASSERT_EQ(value.data(), nullptr);
	ASSERT_EQ(value.size(), 0);
	ASSERT_TRUE(value.empty());
}

TEST(TestMemRef, encodeBody) {
	cql::MemRef value("abc\x00\x01");
	std::string data;
	value.encodeBody(data);
	ASSERT_EQ(data, makeTestString("abc\x00\x01"));
}

TEST(TestMemRef, decodeBody) {
	auto data = makeTestString("abc\x00\x01");
	cql::MemRef value;
	value.decodeBody(data.data(), data.size());
	ASSERT_EQ(std::string(value.data(), value.size()), makeTestString("abc\x00\x01"));
}

