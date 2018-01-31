#include <CqlDriver/Common/ColumnTypes/CqlTime.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlTime, getset) {
	cql::CqlTime value;
	auto now = std::chrono::system_clock::now();
	value.set(now);
	ASSERT_EQ(value.get(), now);

	auto nowTime = std::time(nullptr);
	value.set(nowTime);
	ASSERT_EQ(static_cast<std::time_t>(value), nowTime);

	value.reset();
	ASSERT_EQ(static_cast<std::chrono::nanoseconds>(value).count(), 0);

	std::tm nowTm = {};
	::localtime_r(&nowTime, &nowTm);
	value.set(nowTm);
	std::tm getTm = value;
	ASSERT_EQ(std::mktime(&getTm), std::mktime(&nowTm));

	value = cql::CqlTime();
	ASSERT_EQ(static_cast<std::chrono::nanoseconds>(value).count(), 0);
}

TEST(TestCqlTime, encode) {
	{
		cql::CqlTime value(std::chrono::seconds(123));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x1c\xa3\x5f\x0e\x00"));
	}
	{
		cql::CqlTime value(std::chrono::milliseconds(12345));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x02\xdf\xd1\xc0\x40"));
	}
	{
		auto value = cql::CqlTime::create(1, 2, 3); // 3723s
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x03\x62\xd4\x17\xae\x00"));
	}
}

TEST(TestCqlTime, decode) {
	cql::CqlTime value;
	{
		auto data = makeTestString("\x00\x00\x00\x1c\xa3\x5f\x0e\x00");
		value.decode(data.data(), data.size());
		ASSERT_EQ(static_cast<std::chrono::nanoseconds>(value).count(), 123'000'000'000);
	}
	{
		auto data = makeTestString("\x00\x00\x00\x02\xdf\xd1\xc0\x40");
		value.decode(data.data(), data.size());
		ASSERT_EQ(static_cast<std::chrono::nanoseconds>(value).count(), 12'345'000'000);
	}
	{
		auto data = makeTestString("\x00\x00\x03\x62\xd4\x17\xae\x00");
		value.decode(data.data(), data.size());
		ASSERT_EQ(static_cast<std::chrono::nanoseconds>(value).count(), 3723'000'000'000);
	}
}

TEST(TestCqlTime, operations) {
	{
		// assign and cast
		cql::CqlTime value;
		value = std::chrono::system_clock::now();
		cql::CqlTime::CqlUnderlyingType timeValue = value;
		ASSERT_EQ(timeValue, value.get());

		value = std::chrono::seconds(123);
		std::cout << static_cast<std::chrono::nanoseconds>(value).count() << std::endl;
		ASSERT_EQ(static_cast<std::chrono::seconds>(value).count(), 123);
	}
	{
		// addition
		auto value = cql::CqlTime::create(1, 2, 3);
		ASSERT_EQ(cql::joinString("", value + std::chrono::seconds(2)), "01:02:05.000000000");
	}
	{
		// subtraction
		auto value = cql::CqlTime::create(1, 2, 3);
		ASSERT_EQ(cql::joinString("", value - std::chrono::seconds(2)), "01:02:01.000000000");
	}
	{
		// equal to
		ASSERT_TRUE(cql::CqlTime::create(1, 2, 3) == cql::CqlTime::create(1, 2, 3));
		ASSERT_FALSE(cql::CqlTime::create(1, 2, 3) == cql::CqlTime::create(1, 2, 5));
	}
	{
		// not equal to
		ASSERT_FALSE(cql::CqlTime::create(1, 2, 3) != cql::CqlTime::create(1, 2, 3));
		ASSERT_TRUE(cql::CqlTime::create(1, 2, 3) != cql::CqlTime::create(1, 2, 5));
	}
	{
		// greater than
		ASSERT_TRUE(cql::CqlTime::create(1, 2, 3) > cql::CqlTime::create(1, 2, 1));
		ASSERT_FALSE(cql::CqlTime::create(1, 2, 3) > cql::CqlTime::create(1, 2, 3));
		ASSERT_FALSE(cql::CqlTime::create(1, 2, 3) > cql::CqlTime::create(1, 2, 5));
	}
	{
		// greater or equal than
		ASSERT_TRUE(cql::CqlTime::create(1, 2, 3) >= cql::CqlTime::create(1, 2, 1));
		ASSERT_TRUE(cql::CqlTime::create(1, 2, 3) >= cql::CqlTime::create(1, 2, 3));
		ASSERT_FALSE(cql::CqlTime::create(1, 2, 3) >= cql::CqlTime::create(1, 2, 5));
	}
	{
		// less than
		ASSERT_FALSE(cql::CqlTime::create(1, 2, 3) < cql::CqlTime::create(1, 2, 1));
		ASSERT_FALSE(cql::CqlTime::create(1, 2, 3) < cql::CqlTime::create(1, 2, 3));
		ASSERT_TRUE(cql::CqlTime::create(1, 2, 3) < cql::CqlTime::create(1, 2, 5));
	}
	{
		// less or equal than
		ASSERT_FALSE(cql::CqlTime::create(1, 2, 3) <= cql::CqlTime::create(1, 2, 1));
		ASSERT_TRUE(cql::CqlTime::create(1, 2, 3) <= cql::CqlTime::create(1, 2, 3));
		ASSERT_TRUE(cql::CqlTime::create(1, 2, 3) <= cql::CqlTime::create(1, 2, 5));
	}
	{
		// addition assignment
		auto value = cql::CqlTime::create(1, 2, 3);
		ASSERT_EQ(cql::joinString("", value += std::chrono::seconds(2)), "01:02:05.000000000");
		ASSERT_EQ(cql::joinString("", value += std::chrono::seconds(2)), "01:02:07.000000000");
	}
	{
		// subtraction assignment
		auto value = cql::CqlTime::create(1, 2, 3);
		ASSERT_EQ(cql::joinString("", value -= std::chrono::seconds(2)), "01:02:01.000000000");
		ASSERT_EQ(cql::joinString("", value -= std::chrono::seconds(2)), "01:01:59.000000000");
	}
}

