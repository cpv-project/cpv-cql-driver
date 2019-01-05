#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestInt, getset) {
	cql::Int value(1);
	ASSERT_EQ(value.get(), 1);
	value.set(0x7fff0000);
	ASSERT_EQ(value.get(), 0x7fff0000);

	value = cql::Int(-0x80000000);
	ASSERT_EQ(value.get(), static_cast<std::int32_t>(-0x80000000));
}

TEST(TestInt, encodeBody) {
	cql::Int value(0x12345678);
	std::string data;
	value.encodeBody(data);
	ASSERT_EQ(data, makeTestString("\x12\x34\x56\x78"));
}

TEST(TestInt, decodeBody) {
	{
		cql::Int value(0);
		auto data = makeTestString("\x12\x34\x56\x78");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0x12345678);
	}
	{
		cql::Int value(123);
		auto data = makeTestString("");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.get(), 0);
	}
}

TEST(TestInt, decodeBodyError) {
	cql::Int value(0);
	auto data = makeTestString("\x12");
	ASSERT_THROWS(cql::DecodeException, value.decodeBody(data.data(), data.size()));
}

TEST(TestInt, operations) {
	{
		// assign and cast
		cql::Int value;
		value = 123;
		cql::Int::UnderlyingType intValue = value;
		ASSERT_EQ(intValue, 123);
	}
	{
		// boolean
		ASSERT_TRUE(cql::Int(8));
		ASSERT_FALSE(cql::Int(0));
	}
	{
		// addition
		ASSERT_EQ(cql::Int(1) + cql::Int(2), 3);
		ASSERT_EQ(cql::Int(1) + 2, 3);
		ASSERT_EQ(1 + cql::Int(2), 3);
	}
	{
		// subtractinn
		ASSERT_EQ(cql::Int(3) - cql::Int(2), 1);
		ASSERT_EQ(cql::Int(3) - 2, 1);
		ASSERT_EQ(3 - cql::Int(2), 1);
	}
	{
		// multiplication
		ASSERT_EQ(cql::Int(3) * cql::Int(4), 12);
		ASSERT_EQ(cql::Int(3) * 4, 12);
		ASSERT_EQ(3 * cql::Int(4), 12);
	}
	{
		// division
		ASSERT_EQ(cql::Int(12) / cql::Int(3), 4);
		ASSERT_EQ(cql::Int(12) / 3, 4);
		ASSERT_EQ(12 / cql::Int(3), 4);
	}
	{
		// remainder
		ASSERT_EQ(cql::Int(12) / cql::Int(5), 2);
		ASSERT_EQ(cql::Int(12) / 5, 2);
		ASSERT_EQ(12 / cql::Int(5), 2);
	}
	{
		// bitwise xor
		ASSERT_EQ(cql::Int(13) ^ cql::Int(21), 24);
		ASSERT_EQ(cql::Int(13) ^ 21, 24);
		ASSERT_EQ(13 ^ cql::Int(21), 24);
	}
	{
		// bitwise and
		ASSERT_EQ(cql::Int(13) & cql::Int(21), 5);
		ASSERT_EQ(cql::Int(13) & 21, 5);
		ASSERT_EQ(13 & cql::Int(21), 5);
	}
	{
		// bitwise or
		ASSERT_EQ(cql::Int(13) | cql::Int(21), 29);
		ASSERT_EQ(cql::Int(13) | 21, 29);
		ASSERT_EQ(13 | cql::Int(21), 29);
	}
	{
		// bitwise right shift
		ASSERT_EQ(cql::Int(128) >> cql::Int(3), 16);
		ASSERT_EQ(cql::Int(128) >> 3, 16);
		ASSERT_EQ(128 >> cql::Int(3), 16);
	}
	{
		// bitwise left shift
		ASSERT_EQ(cql::Int(16) << cql::Int(3), 128);
		ASSERT_EQ(cql::Int(16) << 3, 128);
		ASSERT_EQ(16 << cql::Int(3), 128);
	}
	{
		// logical and
		ASSERT_EQ(cql::Int(8) && cql::Int(4), 1);
		ASSERT_EQ(cql::Int(8) && 4, 1);
		ASSERT_EQ(8 && cql::Int(4), 1);
		ASSERT_EQ(cql::Int(3) && cql::Int(0), 0);
		ASSERT_EQ(cql::Int(3) && 0, 0);
		ASSERT_EQ(3 && cql::Int(0), 0);
	}
	{
		// logical or
		ASSERT_EQ(cql::Int(8) || cql::Int(0), 1);
		ASSERT_EQ(cql::Int(8) || 0, 1);
		ASSERT_EQ(8 || cql::Int(0), 1);
		ASSERT_EQ(cql::Int(0) || cql::Int(0), 0);
		ASSERT_EQ(cql::Int(0) || 0, 0);
		ASSERT_EQ(0 || cql::Int(0), 0);
	}
	{
		// unary plus
		ASSERT_EQ(+cql::Int(123), 123);
	}
	{
		// unary minus
		ASSERT_EQ(-cql::Int(123), -123);
	}
	{
		// increment
		cql::Int a(3);
		ASSERT_EQ(++a, 4);
		ASSERT_EQ(a, 4);
		ASSERT_EQ(a++, 4);
		ASSERT_EQ(a, 5);
	}
	{
		// decrement
		cql::Int a(3);
		ASSERT_EQ(--a, 2);
		ASSERT_EQ(a, 2);
		ASSERT_EQ(a--, 2);
		ASSERT_EQ(a, 1);
	}
	{
		// bitwise not
		ASSERT_EQ(~cql::Int(123) & 0xffff, 65412);
	}
	{
		// logical not operation
		ASSERT_EQ(!cql::Int(1), 0);
		ASSERT_EQ(!cql::Int(0), 1);
	}
	{
		// equal to
		ASSERT_TRUE(cql::Int(123) == cql::Int(123));
		ASSERT_TRUE(cql::Int(123) == 123);
		ASSERT_TRUE(123 == cql::Int(123));
		ASSERT_FALSE(cql::Int(123) == cql::Int(321));
		ASSERT_FALSE(cql::Int(123) == 321);
		ASSERT_FALSE(123 == cql::Int(321));
	}
	{
		// not equal to
		ASSERT_FALSE(cql::Int(123) != cql::Int(123));
		ASSERT_FALSE(cql::Int(123) != 123);
		ASSERT_FALSE(123 != cql::Int(123));
		ASSERT_TRUE(cql::Int(123) != cql::Int(321));
		ASSERT_TRUE(cql::Int(123) != 321);
		ASSERT_TRUE(123 != cql::Int(321));
	}
	{
		// greater than
		ASSERT_TRUE(cql::Int(3) > cql::Int(2));
		ASSERT_TRUE(cql::Int(3) > 2);
		ASSERT_TRUE(3 > cql::Int(2));
		ASSERT_FALSE(cql::Int(3) > cql::Int(3));
		ASSERT_FALSE(cql::Int(3) > 3);
		ASSERT_FALSE(3 > cql::Int(3));
		ASSERT_FALSE(cql::Int(2) > cql::Int(3));
		ASSERT_FALSE(cql::Int(2) > 3);
		ASSERT_FALSE(2 > cql::Int(3));
	}
	{
		// greater or equal than
		ASSERT_TRUE(cql::Int(3) >= cql::Int(2));
		ASSERT_TRUE(cql::Int(3) >= 2);
		ASSERT_TRUE(3 >= cql::Int(2));
		ASSERT_TRUE(cql::Int(3) >= cql::Int(3));
		ASSERT_TRUE(cql::Int(3) >= 3);
		ASSERT_TRUE(3 >= cql::Int(3));
		ASSERT_FALSE(cql::Int(2) >= cql::Int(3));
		ASSERT_FALSE(cql::Int(2) >= 3);
		ASSERT_FALSE(2 >= cql::Int(3));
	}
	{
		// less than
		ASSERT_FALSE(cql::Int(3) < cql::Int(2));
		ASSERT_FALSE(cql::Int(3) < 2);
		ASSERT_FALSE(3 < cql::Int(2));
		ASSERT_FALSE(cql::Int(3) < cql::Int(3));
		ASSERT_FALSE(cql::Int(3) < 3);
		ASSERT_FALSE(3 < cql::Int(3));
		ASSERT_TRUE(cql::Int(2) < cql::Int(3));
		ASSERT_TRUE(cql::Int(2) < 3);
		ASSERT_TRUE(2 < cql::Int(3));
	}
	{
		// less or equal than
		ASSERT_FALSE(cql::Int(3) <= cql::Int(2));
		ASSERT_FALSE(cql::Int(3) <= 2);
		ASSERT_FALSE(3 <= cql::Int(2));
		ASSERT_TRUE(cql::Int(3) <= cql::Int(3));
		ASSERT_TRUE(cql::Int(3) <= 3);
		ASSERT_TRUE(3 <= cql::Int(3));
		ASSERT_TRUE(cql::Int(2) <= cql::Int(3));
		ASSERT_TRUE(cql::Int(2) <= 3);
		ASSERT_TRUE(2 <= cql::Int(3));
	}
	{
		// addition assignment
		cql::Int a(1);
		ASSERT_EQ(a += cql::Int(2), 3);
		ASSERT_EQ(a += 2, 5);
		cql::Int::UnderlyingType b(1);
		ASSERT_EQ(b += cql::Int(2), 3);
	}
	{
		// subtraction assignment
		cql::Int a(3);
		ASSERT_EQ(a -= cql::Int(1), 2);
		ASSERT_EQ(a -= 1, 1);
		cql::Int::UnderlyingType b(3);
		ASSERT_EQ(b -= cql::Int(1), 2);
	}
	{
		// multiplication assignment
		cql::Int a(3);
		ASSERT_EQ(a *= cql::Int(4), 12);
		ASSERT_EQ(a *= 4, 48);
		cql::Int::UnderlyingType b(3);
		ASSERT_EQ(b *= cql::Int(4), 12);
	}
	{
		// division assignment
		cql::Int a(48);
		ASSERT_EQ(a /= cql::Int(4), 12);
		ASSERT_EQ(a /= 4, 3);
		cql::Int::UnderlyingType b(12);
		ASSERT_EQ(b /= cql::Int(4), 3);
	}
	{
		// modulo assignment
		cql::Int a(15);
		ASSERT_EQ(a %= cql::Int(4), 3);
		ASSERT_EQ(a %= 2, 1);
		cql::Int::UnderlyingType b(15);
		ASSERT_EQ(b %= cql::Int(4), 3);
	}
	{
		// bitwise xor assignment
		cql::Int a(13);
		ASSERT_EQ(a ^= cql::Int(21), 24);
		ASSERT_EQ(a ^= 13, 21);
		cql::Int::UnderlyingType b(13);
		ASSERT_EQ(b ^= cql::Int(21), 24);
	}
	{
		// bitwise and assignment
		cql::Int a(13);
		ASSERT_EQ(a &= cql::Int(21), 5);
		ASSERT_EQ(a &= 1, 1);
		cql::Int::UnderlyingType b(13);
		ASSERT_EQ(b &= cql::Int(21), 5);
	}
	{
		// bitwise or assignment
		cql::Int a(13);
		ASSERT_EQ(a |= cql::Int(21), 29);
		ASSERT_EQ(a |= 38, 63);
		cql::Int::UnderlyingType b(13);
		ASSERT_EQ(b |= cql::Int(21), 29);
	}
	{
		// bitwise right shift assignment
		cql::Int a(128);
		ASSERT_EQ(a >>= cql::Int(3), 16);
		ASSERT_EQ(a >>= 3, 2);
		cql::Int::UnderlyingType b(128);
		ASSERT_EQ(b >>= cql::Int(3), 16);
	}
	{
		// bitwise left shift assignment
		cql::Int a(2);
		ASSERT_EQ(a <<= cql::Int(3), 16);
		ASSERT_EQ(a <<= 3, 128);
		cql::Int::UnderlyingType b(2);
		ASSERT_EQ(b <<= cql::Int(3), 16);
	}
	{
		// get text description
		ASSERT_EQ(cql::joinString("", cql::Int(123)), "123");
	}
}

