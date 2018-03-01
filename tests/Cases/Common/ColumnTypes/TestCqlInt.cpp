#include <CqlDriver/Common/ColumnTypes/CqlInt.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlInt, getset) {
	cql::CqlInt value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff0000);
	ASSERT_EQ(value.get(), 0x7fff0000);

	value = cql::CqlInt(-0x80000000);
	ASSERT_EQ(value.get(), -0x80000000);
}

TEST(TestCqlInt, encodeBody) {
	cql::CqlInt value(0x12345678);
	seastar::sstring data;
	value.encodeBody(data);
	ASSERT_EQ(data, makeTestString("\x12\x34\x56\x78"));
}

TEST(TestCqlInt, decodeBody) {
	{
		cql::CqlInt value(0);
		auto data = makeTestString("\x12\x34\x56\x78");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x12345678);
	}
	{
		cql::CqlInt value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestCqlInt, decodeBodyError) {
	cql::CqlInt value(0);
	auto data = makeTestString("\x12");
	ASSERT_THROWS(cql::CqlDecodeException, value.decodeBody(data.data(), data.size()));
}

TEST(TestCqlInt, operations) {
	{
		// assign and cast
		cql::CqlInt value;
		value = 123;
		cql::CqlInt::CqlUnderlyingType intValue = value;
		ASSERT_EQ(intValue, 123);
	}
	{
		// boolean
		ASSERT_TRUE(cql::CqlInt(8));
		ASSERT_FALSE(cql::CqlInt(0));
	}
	{
		// addition
		ASSERT_EQ(cql::CqlInt(1) + cql::CqlInt(2), 3);
		ASSERT_EQ(cql::CqlInt(1) + 2, 3);
		ASSERT_EQ(1 + cql::CqlInt(2), 3);
	}
	{
		// subtractinn
		ASSERT_EQ(cql::CqlInt(3) - cql::CqlInt(2), 1);
		ASSERT_EQ(cql::CqlInt(3) - 2, 1);
		ASSERT_EQ(3 - cql::CqlInt(2), 1);
	}
	{
		// multiplication
		ASSERT_EQ(cql::CqlInt(3) * cql::CqlInt(4), 12);
		ASSERT_EQ(cql::CqlInt(3) * 4, 12);
		ASSERT_EQ(3 * cql::CqlInt(4), 12);
	}
	{
		// division
		ASSERT_EQ(cql::CqlInt(12) / cql::CqlInt(3), 4);
		ASSERT_EQ(cql::CqlInt(12) / 3, 4);
		ASSERT_EQ(12 / cql::CqlInt(3), 4);
	}
	{
		// remainder
		ASSERT_EQ(cql::CqlInt(12) / cql::CqlInt(5), 2);
		ASSERT_EQ(cql::CqlInt(12) / 5, 2);
		ASSERT_EQ(12 / cql::CqlInt(5), 2);
	}
	{
		// bitwise xor
		ASSERT_EQ(cql::CqlInt(13) ^ cql::CqlInt(21), 24);
		ASSERT_EQ(cql::CqlInt(13) ^ 21, 24);
		ASSERT_EQ(13 ^ cql::CqlInt(21), 24);
	}
	{
		// bitwise and
		ASSERT_EQ(cql::CqlInt(13) & cql::CqlInt(21), 5);
		ASSERT_EQ(cql::CqlInt(13) & 21, 5);
		ASSERT_EQ(13 & cql::CqlInt(21), 5);
	}
	{
		// bitwise or
		ASSERT_EQ(cql::CqlInt(13) | cql::CqlInt(21), 29);
		ASSERT_EQ(cql::CqlInt(13) | 21, 29);
		ASSERT_EQ(13 | cql::CqlInt(21), 29);
	}
	{
		// bitwise right shift
		ASSERT_EQ(cql::CqlInt(128) >> cql::CqlInt(3), 16);
		ASSERT_EQ(cql::CqlInt(128) >> 3, 16);
		ASSERT_EQ(128 >> cql::CqlInt(3), 16);
	}
	{
		// bitwise left shift
		ASSERT_EQ(cql::CqlInt(16) << cql::CqlInt(3), 128);
		ASSERT_EQ(cql::CqlInt(16) << 3, 128);
		ASSERT_EQ(16 << cql::CqlInt(3), 128);
	}
	{
		// logical and
		ASSERT_EQ(cql::CqlInt(8) && cql::CqlInt(4), 1);
		ASSERT_EQ(cql::CqlInt(8) && 4, 1);
		ASSERT_EQ(8 && cql::CqlInt(4), 1);
		ASSERT_EQ(cql::CqlInt(3) && cql::CqlInt(0), 0);
		ASSERT_EQ(cql::CqlInt(3) && 0, 0);
		ASSERT_EQ(3 && cql::CqlInt(0), 0);
	}
	{
		// logical or
		ASSERT_EQ(cql::CqlInt(8) || cql::CqlInt(0), 1);
		ASSERT_EQ(cql::CqlInt(8) || 0, 1);
		ASSERT_EQ(8 || cql::CqlInt(0), 1);
		ASSERT_EQ(cql::CqlInt(0) || cql::CqlInt(0), 0);
		ASSERT_EQ(cql::CqlInt(0) || 0, 0);
		ASSERT_EQ(0 || cql::CqlInt(0), 0);
	}
	{
		// unary plus
		ASSERT_EQ(+cql::CqlInt(123), 123);
	}
	{
		// unary minus
		ASSERT_EQ(-cql::CqlInt(123), -123);
	}
	{
		// increment
		cql::CqlInt a(3);
		ASSERT_EQ(++a, 4);
		ASSERT_EQ(a, 4);
		ASSERT_EQ(a++, 4);
		ASSERT_EQ(a, 5);
	}
	{
		// decrement
		cql::CqlInt a(3);
		ASSERT_EQ(--a, 2);
		ASSERT_EQ(a, 2);
		ASSERT_EQ(a--, 2);
		ASSERT_EQ(a, 1);
	}
	{
		// bitwise not
		ASSERT_EQ(~cql::CqlInt(123) & 0xffff, 65412);
	}
	{
		// logical not operation
		ASSERT_EQ(!cql::CqlInt(1), 0);
		ASSERT_EQ(!cql::CqlInt(0), 1);
	}
	{
		// equal to
		ASSERT_TRUE(cql::CqlInt(123) == cql::CqlInt(123));
		ASSERT_TRUE(cql::CqlInt(123) == 123);
		ASSERT_TRUE(123 == cql::CqlInt(123));
		ASSERT_FALSE(cql::CqlInt(123) == cql::CqlInt(321));
		ASSERT_FALSE(cql::CqlInt(123) == 321);
		ASSERT_FALSE(123 == cql::CqlInt(321));
	}
	{
		// not equal to
		ASSERT_FALSE(cql::CqlInt(123) != cql::CqlInt(123));
		ASSERT_FALSE(cql::CqlInt(123) != 123);
		ASSERT_FALSE(123 != cql::CqlInt(123));
		ASSERT_TRUE(cql::CqlInt(123) != cql::CqlInt(321));
		ASSERT_TRUE(cql::CqlInt(123) != 321);
		ASSERT_TRUE(123 != cql::CqlInt(321));
	}
	{
		// greater than
		ASSERT_TRUE(cql::CqlInt(3) > cql::CqlInt(2));
		ASSERT_TRUE(cql::CqlInt(3) > 2);
		ASSERT_TRUE(3 > cql::CqlInt(2));
		ASSERT_FALSE(cql::CqlInt(3) > cql::CqlInt(3));
		ASSERT_FALSE(cql::CqlInt(3) > 3);
		ASSERT_FALSE(3 > cql::CqlInt(3));
		ASSERT_FALSE(cql::CqlInt(2) > cql::CqlInt(3));
		ASSERT_FALSE(cql::CqlInt(2) > 3);
		ASSERT_FALSE(2 > cql::CqlInt(3));
	}
	{
		// greater or equal than
		ASSERT_TRUE(cql::CqlInt(3) >= cql::CqlInt(2));
		ASSERT_TRUE(cql::CqlInt(3) >= 2);
		ASSERT_TRUE(3 >= cql::CqlInt(2));
		ASSERT_TRUE(cql::CqlInt(3) >= cql::CqlInt(3));
		ASSERT_TRUE(cql::CqlInt(3) >= 3);
		ASSERT_TRUE(3 >= cql::CqlInt(3));
		ASSERT_FALSE(cql::CqlInt(2) >= cql::CqlInt(3));
		ASSERT_FALSE(cql::CqlInt(2) >= 3);
		ASSERT_FALSE(2 >= cql::CqlInt(3));
	}
	{
		// less than
		ASSERT_FALSE(cql::CqlInt(3) < cql::CqlInt(2));
		ASSERT_FALSE(cql::CqlInt(3) < 2);
		ASSERT_FALSE(3 < cql::CqlInt(2));
		ASSERT_FALSE(cql::CqlInt(3) < cql::CqlInt(3));
		ASSERT_FALSE(cql::CqlInt(3) < 3);
		ASSERT_FALSE(3 < cql::CqlInt(3));
		ASSERT_TRUE(cql::CqlInt(2) < cql::CqlInt(3));
		ASSERT_TRUE(cql::CqlInt(2) < 3);
		ASSERT_TRUE(2 < cql::CqlInt(3));
	}
	{
		// less or equal than
		ASSERT_FALSE(cql::CqlInt(3) <= cql::CqlInt(2));
		ASSERT_FALSE(cql::CqlInt(3) <= 2);
		ASSERT_FALSE(3 <= cql::CqlInt(2));
		ASSERT_TRUE(cql::CqlInt(3) <= cql::CqlInt(3));
		ASSERT_TRUE(cql::CqlInt(3) <= 3);
		ASSERT_TRUE(3 <= cql::CqlInt(3));
		ASSERT_TRUE(cql::CqlInt(2) <= cql::CqlInt(3));
		ASSERT_TRUE(cql::CqlInt(2) <= 3);
		ASSERT_TRUE(2 <= cql::CqlInt(3));
	}
	{
		// addition assignment
		cql::CqlInt a(1);
		ASSERT_EQ(a += cql::CqlInt(2), 3);
		ASSERT_EQ(a += 2, 5);
		cql::CqlInt::CqlUnderlyingType b(1);
		ASSERT_EQ(b += cql::CqlInt(2), 3);
	}
	{
		// subtraction assignment
		cql::CqlInt a(3);
		ASSERT_EQ(a -= cql::CqlInt(1), 2);
		ASSERT_EQ(a -= 1, 1);
		cql::CqlInt::CqlUnderlyingType b(3);
		ASSERT_EQ(b -= cql::CqlInt(1), 2);
	}
	{
		// multiplication assignment
		cql::CqlInt a(3);
		ASSERT_EQ(a *= cql::CqlInt(4), 12);
		ASSERT_EQ(a *= 4, 48);
		cql::CqlInt::CqlUnderlyingType b(3);
		ASSERT_EQ(b *= cql::CqlInt(4), 12);
	}
	{
		// division assignment
		cql::CqlInt a(48);
		ASSERT_EQ(a /= cql::CqlInt(4), 12);
		ASSERT_EQ(a /= 4, 3);
		cql::CqlInt::CqlUnderlyingType b(12);
		ASSERT_EQ(b /= cql::CqlInt(4), 3);
	}
	{
		// modulo assignment
		cql::CqlInt a(15);
		ASSERT_EQ(a %= cql::CqlInt(4), 3);
		ASSERT_EQ(a %= 2, 1);
		cql::CqlInt::CqlUnderlyingType b(15);
		ASSERT_EQ(b %= cql::CqlInt(4), 3);
	}
	{
		// bitwise xor assignment
		cql::CqlInt a(13);
		ASSERT_EQ(a ^= cql::CqlInt(21), 24);
		ASSERT_EQ(a ^= 13, 21);
		cql::CqlInt::CqlUnderlyingType b(13);
		ASSERT_EQ(b ^= cql::CqlInt(21), 24);
	}
	{
		// bitwise and assignment
		cql::CqlInt a(13);
		ASSERT_EQ(a &= cql::CqlInt(21), 5);
		ASSERT_EQ(a &= 1, 1);
		cql::CqlInt::CqlUnderlyingType b(13);
		ASSERT_EQ(b &= cql::CqlInt(21), 5);
	}
	{
		// bitwise or assignment
		cql::CqlInt a(13);
		ASSERT_EQ(a |= cql::CqlInt(21), 29);
		ASSERT_EQ(a |= 38, 63);
		cql::CqlInt::CqlUnderlyingType b(13);
		ASSERT_EQ(b |= cql::CqlInt(21), 29);
	}
	{
		// bitwise right shift assignment
		cql::CqlInt a(128);
		ASSERT_EQ(a >>= cql::CqlInt(3), 16);
		ASSERT_EQ(a >>= 3, 2);
		cql::CqlInt::CqlUnderlyingType b(128);
		ASSERT_EQ(b >>= cql::CqlInt(3), 16);
	}
	{
		// bitwise left shift assignment
		cql::CqlInt a(2);
		ASSERT_EQ(a <<= cql::CqlInt(3), 16);
		ASSERT_EQ(a <<= 3, 128);
		cql::CqlInt::CqlUnderlyingType b(2);
		ASSERT_EQ(b <<= cql::CqlInt(3), 16);
	}
	{
		// get text description
		ASSERT_EQ(cql::joinString("", cql::CqlInt(123)), "123");
	}
}

