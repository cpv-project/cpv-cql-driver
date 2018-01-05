#include <CqlDriver/Exceptions/CqlInternalException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolByte.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolByte, getset) {
	cql::CqlProtocolByte value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(123);
	ASSERT_EQ(value.get(), 123);

	value = cql::CqlProtocolByte(255);
	ASSERT_EQ(value.get(), 255);
}

TEST(TestCqlProtocolByte, encode) {
	cql::CqlProtocolByte value(123);
	seastar::sstring data;
	value.encode(data);
	ASSERT_EQ(data, seastar::sstring("\x7b"));
}

TEST(TestCqlProtocolByte, decode) {
	cql::CqlProtocolByte value(0);
	seastar::sstring data("\x7b");
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	value.decode(ptr, end);
	ASSERT_TRUE(ptr == end);
	ASSERT_EQ(value.get(), 123);
}

TEST(TestCqlProtocolByte, decodeError) {
	cql::CqlProtocolByte value(0);
	seastar::sstring data;
	auto ptr = data.c_str();
	auto end = ptr + data.size();
	ASSERT_THROWS(cql::CqlInternalException, value.decode(ptr, end));
}

