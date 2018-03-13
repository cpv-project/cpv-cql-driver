#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <CQLDriver/Common/ResultSet.hpp>
#include <Common/ResultSetData.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestResultSet, isValid) {
	cql::ResultSet a(cql::makeObject<cql::ResultSetData>(
		0, 0, "", seastar::temporary_buffer<char>(0), 0, 0));
	cql::ResultSet b(cql::makeObject<cql::ResultSetData>(
		0, 0, "", seastar::temporary_buffer<char>(0), 0, 0));
	ASSERT_TRUE(a.isValid());
	ASSERT_TRUE(b.isValid());
	b = std::move(a);
	ASSERT_FALSE(a.isValid());
	ASSERT_TRUE(b.isValid());
	a = std::move(b);
	ASSERT_TRUE(a.isValid());
	ASSERT_FALSE(b.isValid());
}

TEST(TestResultSet, getters) {
	cql::ResultSet resultSet(cql::makeObject<cql::ResultSetData>(
		123, 321, "state", seastar::temporary_buffer<char>("abcde", 5), 1, 3));
	ASSERT_EQ(resultSet.getRowsCount(), 123);
	ASSERT_EQ(resultSet.getColumnsCount(), 321);
	ASSERT_EQ(resultSet.getPagingState(), "state");
	ASSERT_EQ(std::string(resultSet.getDecodePtr(), resultSet.getDecodeEnd()), "bc");
}

TEST(TestResultSet, fill) {
	auto data = makeTestString(
		"\x12\x34\x56\x78"
		"\x00\x00\x00\x04""\x00\x00\x00\x7b"
		"\x00\x00\x00\x03""abc"
		"\x00\x00\x00\x04""\x00\x00\x00\x65"
		"\x00\x00\x00\x05""qwert"
		"\x00");
	cql::ResultSet resultSet(cql::makeObject<cql::ResultSetData>(
		2, 2, "",
		seastar::temporary_buffer<char>(data.data(), data.size()), 4, data.size() - 1));
	cql::Int intValue;
	cql::Text textValue;
	resultSet.fill(intValue, textValue);
	ASSERT_EQ(intValue, 123);
	ASSERT_EQ(textValue, "abc");
	resultSet.fill(intValue, textValue);
	ASSERT_EQ(intValue, 101);
	ASSERT_EQ(textValue, "qwert");
	ASSERT_EQ(resultSet.getDecodePtr(), resultSet.getDecodeEnd());
}

TEST(TestResultSet, error) {
	ASSERT_THROWS_CONTAINS(
		cql::LogicException,
		cql::makeObject<cql::ResultSetData>(0, 0, "", seastar::temporary_buffer<char>(), 0, 0),
		"invalid buffer");
	ASSERT_THROWS_CONTAINS(
		cql::LogicException,
		cql::makeObject<cql::ResultSetData>(0, 0, "", seastar::temporary_buffer<char>(3), 0, 4),
		"invalid to offset");
	ASSERT_THROWS_CONTAINS(
		cql::LogicException,
		cql::makeObject<cql::ResultSetData>(0, 0, "", seastar::temporary_buffer<char>(3), 2, 1),
		"invalid from offset");
}

