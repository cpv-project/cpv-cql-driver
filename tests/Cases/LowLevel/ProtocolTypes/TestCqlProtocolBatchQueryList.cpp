#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolBatchQueryList.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolBatchQueryList, getset) {
	cql::CqlProtocolBatchQueryList value({
		cql::CqlProtocolBatchQuery(),
		cql::CqlProtocolBatchQuery()
	});
	value.get().at(0).setQuery("abc");
	value.get().at(1).setPreparedQueryId("aaaaa");
	ASSERT_EQ(value.get().size(), 2);
	ASSERT_EQ(value.get().at(0).getKind(), cql::CqlBatchQueryKind::Query);
	ASSERT_EQ(value.get().at(0).getQuery(), "abc");
	ASSERT_EQ(value.get().at(1).getKind(), cql::CqlBatchQueryKind::PreparedQueryId);
	ASSERT_EQ(value.get().at(1).getPreparedQueryId(), "aaaaa");

	value = cql::CqlProtocolBatchQueryList();
	ASSERT_TRUE(value.get().empty());
}

TEST(TestCqlProtocolBatchQueryList, encode) {
	cql::CqlProtocolBatchQueryList value({
		cql::CqlProtocolBatchQuery(),
		cql::CqlProtocolBatchQuery()
	});
	value.get().at(0).setQuery("abc");
	value.get().at(1).setPreparedQueryId("aaaaa");
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString(
		"\x00\x02"
		"\x00""\x00\x00\x00\x03""abc""\x00\x00"
		"\x01""\x00\x05""aaaaa""\x00\x00"));
}

TEST(TestCqlProtocolBatchQueryList, decode) {
	cql::CqlProtocolBatchQueryList value;
	{
		auto data = makeTestString(
			"\x00\x02"
			"\x00""\x00\x00\x00\x03""abc""\x00\x00"
			"\x01""\x00\x05""aaaaa""\x00\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get().size(), 2);
		ASSERT_EQ(value.get().at(0).getKind(), cql::CqlBatchQueryKind::Query);
		ASSERT_EQ(value.get().at(0).getQuery(), "abc");
		ASSERT_EQ(value.get().at(1).getKind(), cql::CqlBatchQueryKind::PreparedQueryId);
		ASSERT_EQ(value.get().at(1).getPreparedQueryId(), "aaaaa");
	}
}

TEST(TestCqlProtocolBatchQueryList, decodeError) {
	{
		cql::CqlProtocolBatchQueryList value;
		auto data = makeTestString("\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolBatchQueryList value;
		auto data = makeTestString(
			"\x00\x02"
			"\x00""\x00\x00\x00\x03""abc""\x00\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolBatchQueryList value;
		auto data = makeTestString(
			"\x00\x02"
			"\x00""\x00\x00\x00\x03""abc""\x00\x00"
			"\x01""\x00\x05""aaaaa""\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

