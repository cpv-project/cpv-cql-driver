#include <CqlDriver/Common/ColumnTypes/CqlFloat.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlFloat, getset) {
	cql::CqlFloat value(0.1);
	ASSERT_TRUE(value.approximatelyEquals(0.1));
	value.set(0.2);
	ASSERT_TRUE(value.approximatelyEquals(0.2));

	value = cql::CqlFloat(-12345.67);
	ASSERT_TRUE(value.approximatelyEquals(-12345.67));
}

TEST(TestCqlFloat, approximatelyEquals) {
	cql::CqlFloat value(0.001);
	ASSERT_TRUE(value.approximatelyEquals(0.001));
	ASSERT_FALSE(value.approximatelyEquals(0.00011));

	value.reset();
	ASSERT_TRUE(value.approximatelyEquals(0));
	ASSERT_FALSE(value.approximatelyEquals(0.00001));

	value = 10000.123;
	ASSERT_TRUE(value.approximatelyEquals(10000.123));
	ASSERT_FALSE(value.approximatelyEquals(10000.124));
}

TEST(TestCqlFloat, encode) {
	cql::CqlFloat value(1.1);
	seastar::sstring data;
	value.encode(data);

	float floatValue = 0;
	ASSERT_EQ(sizeof(floatValue), data.size());
	std::memcpy(&floatValue, data.data(), data.size());
	ASSERT_TRUE(value.approximatelyEquals(floatValue));
}

TEST(TestCqlFloat, decode) {
	{
		cql::CqlFloat value(3.0);
		auto data = makeTestString("");
		value.decode(data.data(), data.size());
		ASSERT_TRUE(value.approximatelyEquals(0));
	}
	{
		cql::CqlFloat value;
		auto data = makeTestString("\xcd\xcc\x8c\x3f");
		value.decode(data.data(), data.size());
		ASSERT_TRUE(value.approximatelyEquals(1.1));
	}
}

TEST(TestCqlFloat, operations) {
	{
		// assign and cast
		cql::CqlFloat value;
		value = 12.3;
		cql::CqlFloat::CqlUnderlyingType floatValue = value;
		ASSERT_TRUE(value.approximatelyEquals(floatValue));
	}
	{
		// addition
		cql::CqlFloat::CqlUnderlyingType customEpsilon(0.00001);
		ASSERT_TRUE((cql::CqlFloat(1.3) + cql::CqlFloat(3.2)).approximatelyEquals(4.5, customEpsilon));
		ASSERT_TRUE((cql::CqlFloat(1.3) + 3.2f).approximatelyEquals(4.5, customEpsilon));
		ASSERT_TRUE(cql::CqlFloat(4.5).approximatelyEquals(1.3f + cql::CqlFloat(3.2), customEpsilon));
		ASSERT_TRUE((cql::CqlFloat(1.3) + cql::CqlInt(2)).approximatelyEquals(3.3, customEpsilon));
		ASSERT_TRUE((cql::CqlInt(2) + cql::CqlFloat(1.3)).approximatelyEquals(3.3, customEpsilon));
	}
	{
		// subtraction
		cql::CqlFloat::CqlUnderlyingType customEpsilon(0.00001);
		ASSERT_TRUE((cql::CqlFloat(3.2) - cql::CqlFloat(1.3)).approximatelyEquals(1.9, customEpsilon));
		ASSERT_TRUE((cql::CqlFloat(3.2) - 1.3f).approximatelyEquals(1.9, customEpsilon));
		ASSERT_TRUE(cql::CqlFloat(1.9).approximatelyEquals(3.2f - cql::CqlFloat(1.3), customEpsilon));
		ASSERT_TRUE((cql::CqlFloat(3.2) - cql::CqlInt(2)).approximatelyEquals(1.2, customEpsilon));
		ASSERT_TRUE((cql::CqlInt(3) - cql::CqlFloat(1.2)).approximatelyEquals(1.8, customEpsilon));
	}
	{
		// multiplication
		cql::CqlFloat::CqlUnderlyingType customEpsilon(0.00001);
		ASSERT_TRUE((cql::CqlFloat(1.5) * cql::CqlFloat(2.2)).approximatelyEquals(3.3, customEpsilon));
		ASSERT_TRUE((cql::CqlFloat(1.5) * 2.2f).approximatelyEquals(3.3, customEpsilon));
		ASSERT_TRUE(cql::CqlFloat(3.3).approximatelyEquals(1.5f * cql::CqlFloat(2.2), customEpsilon));
		ASSERT_TRUE((cql::CqlFloat(2.1) * cql::CqlInt(3)).approximatelyEquals(6.3, customEpsilon));
		ASSERT_TRUE((cql::CqlInt(3) * cql::CqlFloat(2.1)).approximatelyEquals(6.3, customEpsilon));
	}
	{
		// division
		cql::CqlFloat::CqlUnderlyingType customEpsilon(0.00001);
		ASSERT_TRUE((cql::CqlFloat(3.3) / cql::CqlFloat(1.5)).approximatelyEquals(2.2, customEpsilon));
		ASSERT_TRUE((cql::CqlFloat(3.3) / 1.5f).approximatelyEquals(2.2, customEpsilon));
		ASSERT_TRUE(cql::CqlFloat(2.2).approximatelyEquals(3.3f / cql::CqlFloat(1.5), customEpsilon));
		ASSERT_TRUE((cql::CqlFloat(3.5) / cql::CqlInt(2)).approximatelyEquals(1.75, customEpsilon));
		ASSERT_TRUE((cql::CqlInt(6) / cql::CqlFloat(0.8)).approximatelyEquals(7.5, customEpsilon));
	}
	{
		// unary plus
		ASSERT_TRUE((+cql::CqlFloat(1.23)).approximatelyEquals(1.23));
	}
	{
		// unary minus
		ASSERT_TRUE((-cql::CqlFloat(1.23)).approximatelyEquals(-1.23));
	}
	{
		// equal to
		ASSERT_TRUE(cql::CqlFloat(1.0) == cql::CqlFloat(1.0));
		ASSERT_TRUE(cql::CqlFloat(1.0) == 1.0f);
		ASSERT_FALSE(cql::CqlFloat(1.0) == cql::CqlFloat(1.1));
		ASSERT_FALSE(cql::CqlFloat(1.0) == 1.1f);
		ASSERT_TRUE(cql::CqlFloat(1.0) == cql::CqlInt(1));
		ASSERT_FALSE(cql::CqlFloat(1.0) == cql::CqlInt(2));
		ASSERT_TRUE(cql::CqlInt(1) == cql::CqlFloat(1.0));
		ASSERT_FALSE(cql::CqlInt(1) == cql::CqlFloat(2.0));
	}
	{
		// not equal to
		ASSERT_FALSE(cql::CqlFloat(1.0) != cql::CqlFloat(1.0));
		ASSERT_FALSE(cql::CqlFloat(1.0) != 1.0f);
		ASSERT_TRUE(cql::CqlFloat(1.0) != cql::CqlFloat(1.1));
		ASSERT_TRUE(cql::CqlFloat(1.0) != 1.1f);
		ASSERT_FALSE(cql::CqlFloat(1.0) != cql::CqlInt(1));
		ASSERT_TRUE(cql::CqlFloat(1.0) != cql::CqlInt(2));
		ASSERT_FALSE(cql::CqlInt(1) != cql::CqlFloat(1.0));
		ASSERT_TRUE(cql::CqlInt(1) != cql::CqlFloat(2.0));
	}
	{
		// greater than
		ASSERT_TRUE(cql::CqlFloat(1.1) > cql::CqlFloat(1.0));
		ASSERT_TRUE(cql::CqlFloat(1.1) > 1.0f);
		ASSERT_FALSE(cql::CqlFloat(1.0) > cql::CqlFloat(1.1));
		ASSERT_FALSE(cql::CqlFloat(1.0) > 1.1f);
		ASSERT_TRUE(cql::CqlFloat(1.1) > cql::CqlInt(1));
		ASSERT_FALSE(cql::CqlFloat(1.1) > cql::CqlInt(2));
		ASSERT_TRUE(cql::CqlInt(2) > cql::CqlFloat(1.1));
		ASSERT_FALSE(cql::CqlInt(1) > cql::CqlFloat(1.1));
	}
	{
		// greater or equal than
		ASSERT_TRUE(cql::CqlFloat(1.1) >= cql::CqlFloat(1.0));
		ASSERT_TRUE(cql::CqlFloat(1.1) >= 1.0f);
		ASSERT_TRUE(cql::CqlFloat(1.0) >= cql::CqlFloat(1.0));
		ASSERT_TRUE(cql::CqlFloat(1.0) >= 1.0f);
		ASSERT_FALSE(cql::CqlFloat(1.0) >= cql::CqlFloat(1.1));
		ASSERT_TRUE(cql::CqlFloat(1.1) >= cql::CqlInt(1));
		ASSERT_FALSE(cql::CqlFloat(1.1) >= cql::CqlInt(2));
		ASSERT_TRUE(cql::CqlInt(2) >= cql::CqlFloat(1.1));
		ASSERT_FALSE(cql::CqlInt(1) >= cql::CqlFloat(1.1));
	}
	{
		// less than
		ASSERT_FALSE(cql::CqlFloat(1.1) < cql::CqlFloat(1.0));
		ASSERT_FALSE(cql::CqlFloat(1.1) < 1.0f);
		ASSERT_TRUE(cql::CqlFloat(1.0) < cql::CqlFloat(1.1));
		ASSERT_TRUE(cql::CqlFloat(1.0) < 1.1f);
		ASSERT_FALSE(cql::CqlFloat(1.1) < cql::CqlInt(1));
		ASSERT_TRUE(cql::CqlFloat(1.1) < cql::CqlInt(2));
		ASSERT_FALSE(cql::CqlInt(2) < cql::CqlFloat(1.1));
		ASSERT_TRUE(cql::CqlInt(1) < cql::CqlFloat(1.1));
	}
	{
		// less or equal than
		ASSERT_FALSE(cql::CqlFloat(1.1) <= cql::CqlFloat(1.0));
		ASSERT_FALSE(cql::CqlFloat(1.1) <= 1.0f);
		ASSERT_TRUE(cql::CqlFloat(1.0) <= cql::CqlFloat(1.0));
		ASSERT_TRUE(cql::CqlFloat(1.0) <= 1.0f);
		ASSERT_TRUE(cql::CqlFloat(1.0) <= cql::CqlFloat(1.1));
		ASSERT_FALSE(cql::CqlFloat(1.1) <= cql::CqlInt(1));
		ASSERT_TRUE(cql::CqlFloat(1.1) <= cql::CqlInt(2));
		ASSERT_FALSE(cql::CqlInt(2) <= cql::CqlFloat(1.1));
		ASSERT_TRUE(cql::CqlInt(1) <= cql::CqlFloat(1.1));
	}
	{
		// addition assignment
		cql::CqlFloat::CqlUnderlyingType customEpsilon(0.00001);
		cql::CqlFloat a;
		ASSERT_TRUE((a += cql::CqlFloat(1.1)).approximatelyEquals(1.1, customEpsilon));
		ASSERT_TRUE((a += 1.1f).approximatelyEquals(2.2, customEpsilon));
		cql::CqlFloat::CqlUnderlyingType b(1.1);
		ASSERT_TRUE(cql::CqlFloat(2.2).approximatelyEquals(b += cql::CqlFloat(1.1), customEpsilon));
	}
	{
		// subtraction assignment
		cql::CqlFloat::CqlUnderlyingType customEpsilon(0.00001);
		cql::CqlFloat a(2.2);
		ASSERT_TRUE((a -= cql::CqlFloat(1.1)).approximatelyEquals(1.1, customEpsilon));
		ASSERT_TRUE((a -= 1.1f).approximatelyEquals(0, customEpsilon));
		cql::CqlFloat::CqlUnderlyingType b(2.2);
		ASSERT_TRUE(cql::CqlFloat(1.1).approximatelyEquals(b -= cql::CqlFloat(1.1), customEpsilon));
	}
	{
		// multiplication assignment
		cql::CqlFloat::CqlUnderlyingType customEpsilon(0.00001);
		cql::CqlFloat a(1.1);
		ASSERT_TRUE((a *= cql::CqlFloat(3)).approximatelyEquals(3.3, customEpsilon));
		ASSERT_TRUE((a *= 3.0f).approximatelyEquals(9.9, customEpsilon));
		cql::CqlFloat::CqlUnderlyingType b(1.1);
		ASSERT_TRUE(cql::CqlFloat(3.3).approximatelyEquals(b *= cql::CqlFloat(3), customEpsilon));
	}
	{
		// division assignment
		cql::CqlFloat::CqlUnderlyingType customEpsilon(0.00001);
		cql::CqlFloat a(9.9);
		ASSERT_TRUE((a /= cql::CqlFloat(3)).approximatelyEquals(3.3, customEpsilon));
		ASSERT_TRUE((a /= 3.0f).approximatelyEquals(1.1, customEpsilon));
		cql::CqlFloat::CqlUnderlyingType b(3.3);
		ASSERT_TRUE(cql::CqlFloat(1.1).approximatelyEquals(b /= cql::CqlFloat(3), customEpsilon));
	}
	{
		// get text description
		std::string str(cql::joinString("", cql::CqlFloat(1.2)));
		ASSERT_EQ(str.find("1.2"), 0);
	}
}

