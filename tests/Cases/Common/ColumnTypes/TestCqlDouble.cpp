#include <CqlDriver/Common/ColumnTypes/CqlDouble.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlDouble, getset) {
	cql::CqlDouble value(0.1);
	ASSERT_TRUE(value.approximatelyEquals(0.1));
	value.set(0.2);
	ASSERT_TRUE(value.approximatelyEquals(0.2));

	value = cql::CqlDouble(-12345.67);
	ASSERT_TRUE(value.approximatelyEquals(-12345.67));
}

TEST(TestCqlDouble, approximatelyEquals) {
	cql::CqlDouble value(0.001);
	ASSERT_TRUE(value.approximatelyEquals(0.001));
	ASSERT_FALSE(value.approximatelyEquals(0.00011));

	value.reset();
	ASSERT_TRUE(value.approximatelyEquals(0));
	ASSERT_FALSE(value.approximatelyEquals(0.00001));

	value = 10000.123;
	ASSERT_TRUE(value.approximatelyEquals(10000.123));
	ASSERT_FALSE(value.approximatelyEquals(10000.124));
}

TEST(TestCqlDouble, encodeBody) {
	cql::CqlDouble value(1.1);
	seastar::sstring data;
	value.encodeBody(data);

	double doubleValue = 0;
	ASSERT_EQ(sizeof(doubleValue), data.size());
	std::memcpy(&doubleValue, data.data(), data.size());
	ASSERT_TRUE(value.approximatelyEquals(doubleValue));
}

TEST(TestCqlDouble, decodeBody) {
	{
		cql::CqlDouble value(3.0);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_TRUE(value.approximatelyEquals(0));
	}
	{
		cql::CqlDouble value;
		auto data = makeTestString("\x9a\x99\x99\x99\x99\x99\xf1\x3f");
		value.decodeBody(data.data(), data.size());
		ASSERT_TRUE(value.approximatelyEquals(1.1));
	}
}

