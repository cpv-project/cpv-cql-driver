#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolConsistency.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolConsistency, getset) {
	cql::ProtocolConsistency value(cql::ConsistencyLevel::One);
	ASSERT_EQ(value.get(), cql::ConsistencyLevel::One);
	value.set(cql::ConsistencyLevel::Quorum);
	ASSERT_EQ(value.get(), cql::ConsistencyLevel::Quorum);

	value = cql::ProtocolConsistency();
	ASSERT_EQ(value.get(), cql::ConsistencyLevel::Any);
}

TEST(TestProtocolConsistency, encode) {
	cql::ProtocolConsistency value(cql::ConsistencyLevel::Three);
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, makeTestString("\x00\x03"));
}

TEST(TestProtocolConsistency, decode) {
	cql::ProtocolConsistency value;
	auto data = makeTestString("\x00\x03");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	value.decode(ptr, end);
	ASSERT_TRUE(ptr == end);
	ASSERT_EQ(value.get(), cql::ConsistencyLevel::Three);
}

TEST(TestProtocolConsistency, decodeError) {
	cql::ProtocolConsistency value;
	auto data = makeTestString("\x00");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
}

