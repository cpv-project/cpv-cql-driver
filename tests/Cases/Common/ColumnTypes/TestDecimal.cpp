#include <CQLDriver/Common/ColumnTypes/Decimal.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestDecimal, getset) {
	cql::Decimal value(0, 123);
	ASSERT_EQ(value.get(), cql::DecimalDataType(0, 123));
	value.set(cql::DecimalDataType(1, 321));
	ASSERT_EQ(value.get(), cql::DecimalDataType(1, 321));

	value = cql::Decimal(-1, 100);
	ASSERT_EQ(value.get(), cql::DecimalDataType(-1, 100));
}

TEST(TestDecimal, scale) {
	{
		cql::Decimal value(2, 123);
		ASSERT_EQ(cql::joinString("", value), "1.23");
		value.scale(3);
		ASSERT_EQ(cql::joinString("", value), "1.230");
		value.scale(1);
		ASSERT_EQ(cql::joinString("", value), "1.2");
	}
	{
		cql::Decimal value(-2, 123);
		ASSERT_EQ(cql::joinString("", value), "12300");
		value.scale(3);
		ASSERT_EQ(cql::joinString("", value), "12300.000");
		value.scale(-1);
		ASSERT_EQ(cql::joinString("", value), "12300");
	}
}

TEST(TestDecimal, strip) {
	{
		cql::Decimal value(5, 12300);
		ASSERT_EQ(cql::joinString("", value), "0.12300");
		value.strip();
		ASSERT_EQ(value.get(), cql::DecimalDataType(3, 123));
		ASSERT_EQ(cql::joinString("", value), "0.123");
	}
	{
		cql::Decimal value;
		value.strip();
		ASSERT_EQ(value.get(), cql::DecimalDataType(0, 0));
	}
}

TEST(TestDecimal, encodeBody) {
	{
		cql::Decimal value;
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00\x00"));
	}
	{
		cql::Decimal value(3, 123);
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x03\x7b"));
	}
	{
		cql::Decimal value(-3, -129);
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\xff\xff\xff\xfd\xff\x7f"));
	}
}

TEST(TestDecimal, decodeBody) {
	cql::Decimal value;
	{
		auto data = makeTestString("\x00\x00\x00\x03\x7b");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), cql::DecimalDataType(3, 123));
	}
	{
		auto data = makeTestString("\xff\xff\xff\xfd\xff\x7f");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), cql::DecimalDataType(-3, -129));
	}
	{
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), cql::DecimalDataType(0, 0));
	}
}

TEST(TestDecimal, decodeBodyError) {
	cql::Decimal value;
	{
		auto data = makeTestString("\x01");
		ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
	}
	{
		auto data = makeTestString("\x00\x00\x00\x01");
		ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
	}
}

TEST(TestDecimal, operations) {
	{
		// assign and cast
		cql::Decimal value;
		value = cql::DecimalDataType(2, 123);
		cql::DecimalDataType decimalValue = value;
		ASSERT_EQ(decimalValue, cql::DecimalDataType(2, 123));
	}
	{
		// addition
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 123) + cql::Decimal(0, 1)), "2.23");
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 123) + cql::Decimal(3, 51)), "1.281");
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 123) + cql::Decimal(2, 321)), "4.44");
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 123) + cql::DecimalDataType(0, -1)), "0.23");
		ASSERT_EQ(cql::joinString("", cql::DecimalDataType(2, -123) + cql::Decimal(0, -1)), "-2.23");
	}
	{
		// subtraction
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 123) - cql::Decimal(0, 1)), "0.23");
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 123) - cql::Decimal(3, 51)), "1.179");
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 123) - cql::Decimal(2, 321)), "-1.98");
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 123) - cql::DecimalDataType(0, 1)), "0.23");
		ASSERT_EQ(cql::joinString("", cql::DecimalDataType(2, 123) - cql::Decimal(-1, 2)), "-18.77");
	}
	{
		// multiplication
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 123) * cql::Decimal(1, 15)), "1.845");
		ASSERT_EQ(cql::joinString("", cql::Decimal(0, -2) * cql::Decimal(3, 15)), "-0.030");
		ASSERT_EQ(cql::joinString("", cql::Decimal(0, -3) * cql::Decimal(0, -5)), "15");
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 123) * cql::DecimalDataType(1, 15)), "1.845");
		ASSERT_EQ(cql::joinString("", cql::DecimalDataType(2, 123) * cql::Decimal(-1, 5)), "61.5");
	}
	{
		// division
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 123) / cql::Decimal(1, 15)), "0.82");
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 20) / cql::Decimal(3, -325)), "-0.61");
		ASSERT_EQ(cql::joinString("", cql::Decimal(3, 123) / cql::Decimal(2, 55)), "0.223");
		ASSERT_EQ(cql::joinString("", cql::Decimal(3, -123) / cql::DecimalDataType(2, -55)), "0.223");
		ASSERT_EQ(cql::joinString("", cql::DecimalDataType(0, 5) / cql::Decimal(0, 2)), "2");
	}
	{
		// unary plus
		ASSERT_EQ(cql::joinString("", +cql::Decimal(2, 123)), "1.23");
		ASSERT_EQ(cql::joinString("", +cql::Decimal(2, -123)), "-1.23");
	}
	{
		// unary minus
		ASSERT_EQ(cql::joinString("", -cql::Decimal(2, 123)), "-1.23");
		ASSERT_EQ(cql::joinString("", -cql::Decimal(2, -123)), "1.23");
	}
	{
		// equal to
		ASSERT_TRUE(cql::Decimal(2, 123) == cql::Decimal(3, 1230));
		ASSERT_TRUE(cql::Decimal(3, 1230) == cql::Decimal(2, 123));
		ASSERT_TRUE(cql::Decimal(2, 123) == cql::Decimal(2, 123));
		ASSERT_FALSE(cql::Decimal(2, 123) == cql::Decimal(3, 1240));
		ASSERT_FALSE(cql::Decimal(3, 1240) == cql::Decimal(2, 123));
		ASSERT_FALSE(cql::Decimal(2, 123) == cql::Decimal(2, 124));
		ASSERT_TRUE(cql::Decimal(0, 200) == cql::Decimal(-2, 2));
		ASSERT_TRUE(cql::Decimal(-2, 2) == cql::Decimal(0, 200));
		ASSERT_FALSE(cql::Decimal(0, 200) == cql::Decimal(-3, 2));
		ASSERT_FALSE(cql::Decimal(-3, 2) == cql::Decimal(0, 200));
		ASSERT_FALSE(cql::Decimal(-3, 2) == cql::DecimalDataType(0, 200));
		ASSERT_FALSE(cql::DecimalDataType(-3, 2) == cql::Decimal(0, 200));
	}
	{
		// not equal to
		ASSERT_FALSE(cql::Decimal(2, 123) != cql::Decimal(3, 1230));
		ASSERT_FALSE(cql::Decimal(3, 1230) != cql::Decimal(2, 123));
		ASSERT_FALSE(cql::Decimal(2, 123) != cql::Decimal(2, 123));
		ASSERT_TRUE(cql::Decimal(2, 123) != cql::Decimal(3, 1240));
		ASSERT_TRUE(cql::Decimal(3, 1240) != cql::Decimal(2, 123));
		ASSERT_TRUE(cql::Decimal(2, 123) != cql::Decimal(2, 124));
		ASSERT_FALSE(cql::Decimal(0, 200) != cql::Decimal(-2, 2));
		ASSERT_FALSE(cql::Decimal(-2, 2) != cql::Decimal(0, 200));
		ASSERT_TRUE(cql::Decimal(0, 200) != cql::Decimal(-3, 2));
		ASSERT_TRUE(cql::Decimal(-3, 2) != cql::Decimal(0, 200));
		ASSERT_TRUE(cql::Decimal(-3, 2) != cql::DecimalDataType(0, 200));
		ASSERT_TRUE(cql::DecimalDataType(-3, 2) != cql::Decimal(0, 200));
	}
	{
		// greater than
		ASSERT_TRUE(cql::Decimal(2, 123) > cql::Decimal(3, 1220));
		ASSERT_FALSE(cql::Decimal(2, 123) > cql::Decimal(3, 1230));
		ASSERT_FALSE(cql::Decimal(2, 123) > cql::Decimal(3, 1240));
		ASSERT_TRUE(cql::Decimal(3, 1230) > cql::Decimal(2, 122));
		ASSERT_FALSE(cql::Decimal(3, 1230) > cql::Decimal(2, 123));
		ASSERT_FALSE(cql::Decimal(3, 1230) > cql::Decimal(2, 124));
		ASSERT_FALSE(cql::Decimal(3, 1230) > cql::DecimalDataType(2, 124));
		ASSERT_FALSE(cql::DecimalDataType(3, 1230) > cql::Decimal(2, 124));
	}
	{
		// greater or equal than
		ASSERT_TRUE(cql::Decimal(2, 123) >= cql::Decimal(3, 1220));
		ASSERT_TRUE(cql::Decimal(2, 123) >= cql::Decimal(3, 1230));
		ASSERT_FALSE(cql::Decimal(2, 123) >= cql::Decimal(3, 1240));
		ASSERT_TRUE(cql::Decimal(3, 1230) >= cql::Decimal(2, 122));
		ASSERT_TRUE(cql::Decimal(3, 1230) >= cql::Decimal(2, 123));
		ASSERT_FALSE(cql::Decimal(3, 1230) >= cql::Decimal(2, 124));
		ASSERT_FALSE(cql::Decimal(3, 1230) >= cql::DecimalDataType(2, 124));
		ASSERT_FALSE(cql::DecimalDataType(3, 1230) >= cql::Decimal(2, 124));
	}
	{
		// less than
		ASSERT_FALSE(cql::Decimal(2, 123) < cql::Decimal(3, 1220));
		ASSERT_FALSE(cql::Decimal(2, 123) < cql::Decimal(3, 1230));
		ASSERT_TRUE(cql::Decimal(2, 123) < cql::Decimal(3, 1240));
		ASSERT_FALSE(cql::Decimal(3, 1230) < cql::Decimal(2, 122));
		ASSERT_FALSE(cql::Decimal(3, 1230) < cql::Decimal(2, 123));
		ASSERT_TRUE(cql::Decimal(3, 1230) < cql::Decimal(2, 124));
		ASSERT_TRUE(cql::Decimal(3, 1230) < cql::DecimalDataType(2, 124));
		ASSERT_TRUE(cql::DecimalDataType(3, 1230) < cql::Decimal(2, 124));
	}
	{
		// less or equal than
		ASSERT_FALSE(cql::Decimal(2, 123) <= cql::Decimal(3, 1220));
		ASSERT_TRUE(cql::Decimal(2, 123) <= cql::Decimal(3, 1230));
		ASSERT_TRUE(cql::Decimal(2, 123) <= cql::Decimal(3, 1240));
		ASSERT_FALSE(cql::Decimal(3, 1230) <= cql::Decimal(2, 122));
		ASSERT_TRUE(cql::Decimal(3, 1230) <= cql::Decimal(2, 123));
		ASSERT_TRUE(cql::Decimal(3, 1230) <= cql::Decimal(2, 124));
		ASSERT_TRUE(cql::Decimal(3, 1230) <= cql::DecimalDataType(2, 124));
		ASSERT_TRUE(cql::DecimalDataType(3, 1230) <= cql::Decimal(2, 124));
	}
	{
		// addition assignment
		cql::Decimal value(2, 123);
		ASSERT_EQ(cql::joinString("", value += cql::Decimal(1, 21)), "3.33");
		ASSERT_EQ(cql::joinString("", value += cql::DecimalDataType(3, 51)), "3.381");
		cql::DecimalDataType decimalValue(2, 123);
		decimalValue += cql::Decimal(2, 321);
		ASSERT_EQ(cql::joinString("", cql::Decimal(decimalValue)), "4.44");
	}
	{
		// subtraction assignment
		cql::Decimal value(2, 123);
		ASSERT_EQ(cql::joinString("", value -= cql::Decimal(1, 21)), "-0.87");
		ASSERT_EQ(cql::joinString("", value -= cql::DecimalDataType(3, -51)), "-0.819");
		cql::DecimalDataType decimalValue(2, 123);
		decimalValue -= cql::Decimal(1, 21);
		ASSERT_EQ(cql::joinString("", cql::Decimal(decimalValue)), "-0.87");
	}
	{
		// multiplication assignment
		cql::Decimal value(2, 123);
		ASSERT_EQ(cql::joinString("", value *= cql::Decimal(1, 15)), "1.845");
		ASSERT_EQ(cql::joinString("", value *= cql::DecimalDataType(-1, 8)), "147.60");
		cql::DecimalDataType decimalValue(2, 123);
		decimalValue *= cql::Decimal(1, 15);
		ASSERT_EQ(cql::joinString("", cql::Decimal(decimalValue)), "1.845");
	}
	{
		// division assignment
		cql::Decimal value(2, 123);
		ASSERT_EQ(cql::joinString("", value /= cql::Decimal(1, 15)), "0.82");
		ASSERT_EQ(cql::joinString("", value /= cql::DecimalDataType(-1, 8)), "0.01");
		cql::DecimalDataType decimalValue(2, 123);
		decimalValue /= cql::Decimal(1, 15);
		ASSERT_EQ(cql::joinString("", cql::Decimal(decimalValue)), "0.82");
	}
	{
		// get text decription
		ASSERT_EQ(cql::joinString("", cql::Decimal(0, 123)), "123");
		ASSERT_EQ(cql::joinString("", cql::Decimal(0, -123)), "-123");
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, 123)), "1.23");
		ASSERT_EQ(cql::joinString("", cql::Decimal(2, -123)), "-1.23");
		ASSERT_EQ(cql::joinString("", cql::Decimal(-2, 123)), "12300");
		ASSERT_EQ(cql::joinString("", cql::Decimal(-2, -123)), "-12300");
	}
}

