#include <LowLevel/ProtocolTypes/CqlProtocolShort.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolShort, all) {
	cql::CqlProtocolShort value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(12321);
	ASSERT_EQ(value.get(), 12321);

	value = cql::CqlProtocolShort(32100);
	ASSERT_EQ(value.get(), 32100);
}

TEST(TestCqlProtocolShort, encode) {
	cql::CqlProtocolShort value(0x1234);
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, seastar::sstring("\x12\x34"));
}

TEST(TestCqlProtocolShort, decode) {
	cql::CqlProtocolShort value(0);
	seastar::sstring data("\x12\x34");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	value.decode(ptr, end);
	ASSERT_TRUE(ptr == end);
	ASSERT_EQ(value.get(), 0x1234);
}

