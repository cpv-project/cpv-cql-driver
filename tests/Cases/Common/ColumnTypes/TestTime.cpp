#include <CQLDriver/Common/ColumnTypes/Time.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestTime, getset) {
	cql::Time value;
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

	value = cql::Time();
	ASSERT_EQ(static_cast<std::chrono::nanoseconds>(value).count(), 0);
}

TEST(TestTime, encodeBody) {
	{
		cql::Time value(std::chrono::seconds(123));
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x1c\xa3\x5f\x0e\x00"));
	}
	{
		cql::Time value(std::chrono::milliseconds(12345));
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x02\xdf\xd1\xc0\x40"));
	}
	{
		auto value = cql::Time::create(1, 2, 3); // 3723s
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x03\x62\xd4\x17\xae\x00"));
	}
}

TEST(TestTime, decodeBody) {
	cql::Time value;
	{
		auto data = makeTestString("\x00\x00\x00\x1c\xa3\x5f\x0e\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(static_cast<std::chrono::nanoseconds>(value).count(), 123'000'000'000);
	}
	{
		auto data = makeTestString("\x00\x00\x00\x02\xdf\xd1\xc0\x40");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(static_cast<std::chrono::nanoseconds>(value).count(), 12'345'000'000);
	}
	{
		auto data = makeTestString("\x00\x00\x03\x62\xd4\x17\xae\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(static_cast<std::chrono::nanoseconds>(value).count(), 3723'000'000'000);
	}
}

TEST(TestTime, operations) {
	{
		// assign and cast
		cql::Time value;
		value = std::chrono::system_clock::now();
		cql::Time::UnderlyingType timeValue = value;
		ASSERT_EQ(timeValue, value.get());

		value = std::chrono::seconds(123);
		ASSERT_EQ(static_cast<std::chrono::seconds>(value).count(), 123);
	}
	{
		// addition
		auto value = cql::Time::create(1, 2, 3);
		ASSERT_EQ(cql::joinString("", value + std::chrono::seconds(2)), "01:02:05.000000000");
	}
	{
		// subtraction
		auto value = cql::Time::create(1, 2, 3);
		ASSERT_EQ(cql::joinString("", value - std::chrono::seconds(2)), "01:02:01.000000000");
	}
	{
		// difference
		auto a = cql::Time::create(1, 2, 3);
		auto b = cql::Time::create(1, 0, 3);
		ASSERT_EQ(std::chrono::duration_cast<std::chrono::milliseconds>(a - b).count(), 120'000);
		ASSERT_EQ(std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count(), -120'000);
	}
	{
		// equal to
		ASSERT_TRUE(cql::Time::create(1, 2, 3) == cql::Time::create(1, 2, 3));
		ASSERT_FALSE(cql::Time::create(1, 2, 3) == cql::Time::create(1, 2, 5));
	}
	{
		// not equal to
		ASSERT_FALSE(cql::Time::create(1, 2, 3) != cql::Time::create(1, 2, 3));
		ASSERT_TRUE(cql::Time::create(1, 2, 3) != cql::Time::create(1, 2, 5));
	}
	{
		// greater than
		ASSERT_TRUE(cql::Time::create(1, 2, 3) > cql::Time::create(1, 2, 1));
		ASSERT_FALSE(cql::Time::create(1, 2, 3) > cql::Time::create(1, 2, 3));
		ASSERT_FALSE(cql::Time::create(1, 2, 3) > cql::Time::create(1, 2, 5));
	}
	{
		// greater or equal than
		ASSERT_TRUE(cql::Time::create(1, 2, 3) >= cql::Time::create(1, 2, 1));
		ASSERT_TRUE(cql::Time::create(1, 2, 3) >= cql::Time::create(1, 2, 3));
		ASSERT_FALSE(cql::Time::create(1, 2, 3) >= cql::Time::create(1, 2, 5));
	}
	{
		// less than
		ASSERT_FALSE(cql::Time::create(1, 2, 3) < cql::Time::create(1, 2, 1));
		ASSERT_FALSE(cql::Time::create(1, 2, 3) < cql::Time::create(1, 2, 3));
		ASSERT_TRUE(cql::Time::create(1, 2, 3) < cql::Time::create(1, 2, 5));
	}
	{
		// less or equal than
		ASSERT_FALSE(cql::Time::create(1, 2, 3) <= cql::Time::create(1, 2, 1));
		ASSERT_TRUE(cql::Time::create(1, 2, 3) <= cql::Time::create(1, 2, 3));
		ASSERT_TRUE(cql::Time::create(1, 2, 3) <= cql::Time::create(1, 2, 5));
	}
	{
		// addition assignment
		auto value = cql::Time::create(1, 2, 3);
		ASSERT_EQ(cql::joinString("", value += std::chrono::seconds(2)), "01:02:05.000000000");
		ASSERT_EQ(cql::joinString("", value += std::chrono::seconds(2)), "01:02:07.000000000");
	}
	{
		// subtraction assignment
		auto value = cql::Time::create(1, 2, 3);
		ASSERT_EQ(cql::joinString("", value -= std::chrono::seconds(2)), "01:02:01.000000000");
		ASSERT_EQ(cql::joinString("", value -= std::chrono::seconds(2)), "01:01:59.000000000");
	}
}

