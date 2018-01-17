#include <LowLevel/ProtocolTypes/CqlProtocolBatchQuery.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolBatchQuery, getset) {
	{
		cql::CqlProtocolBatchQuery value;
		value.setQuery("abc");
		ASSERT_EQ(value.getKind(), cql::CqlBatchQueryKind::Query);
		ASSERT_EQ(value.getQuery(), "abc");
	}
	{
		cql::CqlProtocolBatchQuery value;
		value.setPreparedQueryId("aaa");
		value.setValues({ cql::CqlProtocolValue("a"), cql::CqlProtocolValue("b") });
		ASSERT_EQ(value.getKind(), cql::CqlBatchQueryKind::PreparedQueryId);
		ASSERT_EQ(value.getPreparedQueryId(), "aaa");
		ASSERT_EQ(value.getValues().size(), 2);
		ASSERT_EQ(value.getValues().at(0).get(), "a");
		ASSERT_EQ(value.getValues().at(1).get(), "b");
	}
}

TEST(TestCqlProtocolBatchQuery, encode) {
	{
		cql::CqlProtocolBatchQuery value;
		value.setQuery("abc");
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00"
			"\x00\x00\x00\x03""abc"
			"\x00\x00"));
	}
	{
		cql::CqlProtocolBatchQuery value;
		value.setPreparedQueryId("aaa");
		value.setValues({ cql::CqlProtocolValue("a"), cql::CqlProtocolValue("b") });
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x01"
			"\x00\x03""aaa"
			"\x00\x02"
			"\x00\x00\x00\x01""a"
			"\x00\x00\x00\x01""b"));
	}
}

TEST(TestCqlProtocolBatchQuery, decode) {
	cql::CqlProtocolBatchQuery value;
	{
		auto data = makeTestString(
			"\x00"
			"\x00\x00\x00\x03""abc"
			"\x00\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getKind(), cql::CqlBatchQueryKind::Query);
		ASSERT_EQ(value.getQuery(), "abc");
	}
	{
		auto data = makeTestString(
			"\x01"
			"\x00\x03""aaa"
			"\x00\x02"
			"\x00\x00\x00\x01""a"
			"\x00\x00\x00\x01""b");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getKind(), cql::CqlBatchQueryKind::PreparedQueryId);
		ASSERT_EQ(value.getPreparedQueryId(), "aaa");
		ASSERT_EQ(value.getValues().size(), 2);
		ASSERT_EQ(value.getValues().at(0).get(), "a");
		ASSERT_EQ(value.getValues().at(1).get(), "b");
	}
}

