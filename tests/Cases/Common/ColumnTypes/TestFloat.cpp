#include <CQLDriver/Common/ColumnTypes/Float.hpp>
#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestFloat, getset) {
	cql::Float value(0.1);
	ASSERT_TRUE(value.approximatelyEquals(0.1));
	value.set(0.2);
	ASSERT_TRUE(value.approximatelyEquals(0.2));

	value = cql::Float(-12345.67);
	ASSERT_TRUE(value.approximatelyEquals(-12345.67));
}

TEST(TestFloat, approximatelyEquals) {
	cql::Float value(0.001);
	ASSERT_TRUE(value.approximatelyEquals(0.001));
	ASSERT_FALSE(value.approximatelyEquals(0.00011));

	value.reset();
	ASSERT_TRUE(value.approximatelyEquals(0));
	ASSERT_FALSE(value.approximatelyEquals(0.00001));

	value = 10000.123;
	ASSERT_TRUE(value.approximatelyEquals(10000.123));
	ASSERT_FALSE(value.approximatelyEquals(10000.124));
}

TEST(TestFloat, encodeBody) {
	cql::Float value(1.1);
	std::string data;
	value.encodeBody(data);

	float floatValue = 0;
	ASSERT_EQ(sizeof(floatValue), data.size());
	std::memcpy(&floatValue, data.data(), data.size());
	ASSERT_TRUE(value.approximatelyEquals(floatValue));
}

TEST(TestFloat, decodeBody) {
	{
		cql::Float value(3.0);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_TRUE(value.approximatelyEquals(0));
	}
	{
		cql::Float value;
		auto data = makeTestString("\xcd\xcc\x8c\x3f");
		value.decodeBody(data.data(), data.size());
		ASSERT_TRUE(value.approximatelyEquals(1.1));
	}
}

TEST(TestFloat, operations) {
	{
		// assign and cast
		cql::Float value;
		value = 12.3;
		cql::Float::UnderlyingType floatValue = value;
		ASSERT_TRUE(value.approximatelyEquals(floatValue));
	}
	{
		// addition
		cql::Float::UnderlyingType customEpsilon(0.00001);
		ASSERT_TRUE((cql::Float(1.3) + cql::Float(3.2)).approximatelyEquals(4.5, customEpsilon));
		ASSERT_TRUE((cql::Float(1.3) + 3.2f).approximatelyEquals(4.5, customEpsilon));
		ASSERT_TRUE(cql::Float(4.5).approximatelyEquals(1.3f + cql::Float(3.2), customEpsilon));
		ASSERT_TRUE((cql::Float(1.3) + cql::Int(2)).approximatelyEquals(3.3, customEpsilon));
		ASSERT_TRUE((cql::Int(2) + cql::Float(1.3)).approximatelyEquals(3.3, customEpsilon));
	}
	{
		// subtraction
		cql::Float::UnderlyingType customEpsilon(0.00001);
		ASSERT_TRUE((cql::Float(3.2) - cql::Float(1.3)).approximatelyEquals(1.9, customEpsilon));
		ASSERT_TRUE((cql::Float(3.2) - 1.3f).approximatelyEquals(1.9, customEpsilon));
		ASSERT_TRUE(cql::Float(1.9).approximatelyEquals(3.2f - cql::Float(1.3), customEpsilon));
		ASSERT_TRUE((cql::Float(3.2) - cql::Int(2)).approximatelyEquals(1.2, customEpsilon));
		ASSERT_TRUE((cql::Int(3) - cql::Float(1.2)).approximatelyEquals(1.8, customEpsilon));
	}
	{
		// multiplication
		cql::Float::UnderlyingType customEpsilon(0.00001);
		ASSERT_TRUE((cql::Float(1.5) * cql::Float(2.2)).approximatelyEquals(3.3, customEpsilon));
		ASSERT_TRUE((cql::Float(1.5) * 2.2f).approximatelyEquals(3.3, customEpsilon));
		ASSERT_TRUE(cql::Float(3.3).approximatelyEquals(1.5f * cql::Float(2.2), customEpsilon));
		ASSERT_TRUE((cql::Float(2.1) * cql::Int(3)).approximatelyEquals(6.3, customEpsilon));
		ASSERT_TRUE((cql::Int(3) * cql::Float(2.1)).approximatelyEquals(6.3, customEpsilon));
	}
	{
		// division
		cql::Float::UnderlyingType customEpsilon(0.00001);
		ASSERT_TRUE((cql::Float(3.3) / cql::Float(1.5)).approximatelyEquals(2.2, customEpsilon));
		ASSERT_TRUE((cql::Float(3.3) / 1.5f).approximatelyEquals(2.2, customEpsilon));
		ASSERT_TRUE(cql::Float(2.2).approximatelyEquals(3.3f / cql::Float(1.5), customEpsilon));
		ASSERT_TRUE((cql::Float(3.5) / cql::Int(2)).approximatelyEquals(1.75, customEpsilon));
		ASSERT_TRUE((cql::Int(6) / cql::Float(0.8)).approximatelyEquals(7.5, customEpsilon));
	}
	{
		// unary plus
		ASSERT_TRUE((+cql::Float(1.23)).approximatelyEquals(1.23));
	}
	{
		// unary minus
		ASSERT_TRUE((-cql::Float(1.23)).approximatelyEquals(-1.23));
	}
	{
		// equal to
		ASSERT_TRUE(cql::Float(1.0) == cql::Float(1.0));
		ASSERT_TRUE(cql::Float(1.0) == 1.0f);
		ASSERT_FALSE(cql::Float(1.0) == cql::Float(1.1));
		ASSERT_FALSE(cql::Float(1.0) == 1.1f);
		ASSERT_TRUE(cql::Float(1.0) == cql::Int(1));
		ASSERT_FALSE(cql::Float(1.0) == cql::Int(2));
		ASSERT_TRUE(cql::Int(1) == cql::Float(1.0));
		ASSERT_FALSE(cql::Int(1) == cql::Float(2.0));
	}
	{
		// not equal to
		ASSERT_FALSE(cql::Float(1.0) != cql::Float(1.0));
		ASSERT_FALSE(cql::Float(1.0) != 1.0f);
		ASSERT_TRUE(cql::Float(1.0) != cql::Float(1.1));
		ASSERT_TRUE(cql::Float(1.0) != 1.1f);
		ASSERT_FALSE(cql::Float(1.0) != cql::Int(1));
		ASSERT_TRUE(cql::Float(1.0) != cql::Int(2));
		ASSERT_FALSE(cql::Int(1) != cql::Float(1.0));
		ASSERT_TRUE(cql::Int(1) != cql::Float(2.0));
	}
	{
		// greater than
		ASSERT_TRUE(cql::Float(1.1) > cql::Float(1.0));
		ASSERT_TRUE(cql::Float(1.1) > 1.0f);
		ASSERT_FALSE(cql::Float(1.0) > cql::Float(1.1));
		ASSERT_FALSE(cql::Float(1.0) > 1.1f);
		ASSERT_TRUE(cql::Float(1.1) > cql::Int(1));
		ASSERT_FALSE(cql::Float(1.1) > cql::Int(2));
		ASSERT_TRUE(cql::Int(2) > cql::Float(1.1));
		ASSERT_FALSE(cql::Int(1) > cql::Float(1.1));
	}
	{
		// greater or equal than
		ASSERT_TRUE(cql::Float(1.1) >= cql::Float(1.0));
		ASSERT_TRUE(cql::Float(1.1) >= 1.0f);
		ASSERT_TRUE(cql::Float(1.0) >= cql::Float(1.0));
		ASSERT_TRUE(cql::Float(1.0) >= 1.0f);
		ASSERT_FALSE(cql::Float(1.0) >= cql::Float(1.1));
		ASSERT_TRUE(cql::Float(1.1) >= cql::Int(1));
		ASSERT_FALSE(cql::Float(1.1) >= cql::Int(2));
		ASSERT_TRUE(cql::Int(2) >= cql::Float(1.1));
		ASSERT_FALSE(cql::Int(1) >= cql::Float(1.1));
	}
	{
		// less than
		ASSERT_FALSE(cql::Float(1.1) < cql::Float(1.0));
		ASSERT_FALSE(cql::Float(1.1) < 1.0f);
		ASSERT_TRUE(cql::Float(1.0) < cql::Float(1.1));
		ASSERT_TRUE(cql::Float(1.0) < 1.1f);
		ASSERT_FALSE(cql::Float(1.1) < cql::Int(1));
		ASSERT_TRUE(cql::Float(1.1) < cql::Int(2));
		ASSERT_FALSE(cql::Int(2) < cql::Float(1.1));
		ASSERT_TRUE(cql::Int(1) < cql::Float(1.1));
	}
	{
		// less or equal than
		ASSERT_FALSE(cql::Float(1.1) <= cql::Float(1.0));
		ASSERT_FALSE(cql::Float(1.1) <= 1.0f);
		ASSERT_TRUE(cql::Float(1.0) <= cql::Float(1.0));
		ASSERT_TRUE(cql::Float(1.0) <= 1.0f);
		ASSERT_TRUE(cql::Float(1.0) <= cql::Float(1.1));
		ASSERT_FALSE(cql::Float(1.1) <= cql::Int(1));
		ASSERT_TRUE(cql::Float(1.1) <= cql::Int(2));
		ASSERT_FALSE(cql::Int(2) <= cql::Float(1.1));
		ASSERT_TRUE(cql::Int(1) <= cql::Float(1.1));
	}
	{
		// addition assignment
		cql::Float::UnderlyingType customEpsilon(0.00001);
		cql::Float a;
		ASSERT_TRUE((a += cql::Float(1.1)).approximatelyEquals(1.1, customEpsilon));
		ASSERT_TRUE((a += 1.1f).approximatelyEquals(2.2, customEpsilon));
		cql::Float::UnderlyingType b(1.1);
		ASSERT_TRUE(cql::Float(2.2).approximatelyEquals(b += cql::Float(1.1), customEpsilon));
	}
	{
		// subtraction assignment
		cql::Float::UnderlyingType customEpsilon(0.00001);
		cql::Float a(2.2);
		ASSERT_TRUE((a -= cql::Float(1.1)).approximatelyEquals(1.1, customEpsilon));
		ASSERT_TRUE((a -= 1.1f).approximatelyEquals(0, customEpsilon));
		cql::Float::UnderlyingType b(2.2);
		ASSERT_TRUE(cql::Float(1.1).approximatelyEquals(b -= cql::Float(1.1), customEpsilon));
	}
	{
		// multiplication assignment
		cql::Float::UnderlyingType customEpsilon(0.00001);
		cql::Float a(1.1);
		ASSERT_TRUE((a *= cql::Float(3)).approximatelyEquals(3.3, customEpsilon));
		ASSERT_TRUE((a *= 3.0f).approximatelyEquals(9.9, customEpsilon));
		cql::Float::UnderlyingType b(1.1);
		ASSERT_TRUE(cql::Float(3.3).approximatelyEquals(b *= cql::Float(3), customEpsilon));
	}
	{
		// division assignment
		cql::Float::UnderlyingType customEpsilon(0.00001);
		cql::Float a(9.9);
		ASSERT_TRUE((a /= cql::Float(3)).approximatelyEquals(3.3, customEpsilon));
		ASSERT_TRUE((a /= 3.0f).approximatelyEquals(1.1, customEpsilon));
		cql::Float::UnderlyingType b(3.3);
		ASSERT_TRUE(cql::Float(1.1).approximatelyEquals(b /= cql::Float(3), customEpsilon));
	}
	{
		// get text description
		std::string str(cql::joinString("", cql::Float(1.2)));
		ASSERT_EQ(str.find("1.2"), 0);
	}
}

