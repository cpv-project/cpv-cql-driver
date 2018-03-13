#include <CQLDriver/Common/ColumnTypes/Date.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestDate, getset) {
	cql::Date value;
	auto now = std::chrono::system_clock::now();
	value.set(now);
	ASSERT_EQ(value.get(), now);

	auto nowTime = std::time(nullptr);
	value.set(nowTime);
	ASSERT_EQ(static_cast<std::time_t>(value), nowTime);

	value.reset();
	ASSERT_EQ(value.get().time_since_epoch().count(), 0);

	std::tm nowTm = {};
	::localtime_r(&nowTime, &nowTm);
	value.set(nowTm);
	std::tm getTm = value;
	ASSERT_EQ(std::mktime(&getTm), std::mktime(&nowTm));
}

TEST(TestDate, encodeBody) {
	{
		auto value = cql::Date::create(1970, 1, 1);
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x80\x00\x00\x00"));
	}
	{
		auto value = cql::Date::create(1970, 1, 2);
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x80\x00\x00\x01"));
	}
	{
		auto value = cql::Date::create(1969, 12, 31);
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x7f\xff\xff\xff"));
	}
}

TEST(TestDate, decodeBody) {
	cql::Date value;
	{
		auto data = makeTestString("\x80\x00\x00\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(cql::joinString("", value), "1970-01-01");
	}
	{
		auto data = makeTestString("\x80\x00\x00\x01");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(cql::joinString("", value), "1970-01-02");
	}
	{
		auto data = makeTestString("\x7f\xff\xff\xff");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(cql::joinString("", value), "1969-12-31");
	}
}

TEST(TestDate, operations) {
	{
		// assign and cast
		cql::Date value;
		value = std::chrono::system_clock::now();
		cql::Date::UnderlyingType dateValue = value;
		ASSERT_EQ(dateValue, value.get());
	}
	{
		// addition
		auto value = cql::Date::create(1970, 1, 1);
		auto dayAfter = value + std::chrono::hours(24);
		auto dayBefore = value + std::chrono::hours(-24);
		auto sameDate = value + std::chrono::hours(23);
		ASSERT_EQ(cql::joinString("", dayAfter), "1970-01-02");
		ASSERT_EQ(cql::joinString("", dayBefore), "1969-12-31");
		ASSERT_EQ(cql::joinString("", sameDate), "1970-01-01");
	}
	{
		// subtractinn
		auto value = cql::Date::create(1970, 1, 15);
		auto dayBefore = value - std::chrono::hours(24);
		ASSERT_EQ(cql::joinString("", dayBefore), "1970-01-14");
	}
	{
		// difference
		auto a = cql::Date::create(1970, 1, 3);
		auto b = cql::Date::create(1970, 1, 1);
		ASSERT_EQ(std::chrono::duration_cast<std::chrono::hours>(a - b).count(), 48);
		ASSERT_EQ(std::chrono::duration_cast<std::chrono::hours>(b - a).count(), -48);
	}
	{
		// equal to
		ASSERT_TRUE(cql::Date::create(1970, 1, 1) == cql::Date::create(1970, 1, 1));
		ASSERT_FALSE(cql::Date::create(1970, 1, 1) == cql::Date::create(1970, 1, 2));
		auto now = std::chrono::system_clock::now();
		ASSERT_TRUE(cql::Date(now) == now);
		ASSERT_FALSE(now == cql::Date(now) + std::chrono::seconds(1));
	}
	{
		// not equal to
		ASSERT_FALSE(cql::Date::create(1970, 1, 1) != cql::Date::create(1970, 1, 1));
		ASSERT_TRUE(cql::Date::create(1970, 1, 1) != cql::Date::create(1970, 1, 2));
		auto now = std::chrono::system_clock::now();
		ASSERT_FALSE(cql::Date(now) != now);
		ASSERT_TRUE(now != cql::Date(now) + std::chrono::seconds(1));
	}
	{
		// greater than
		ASSERT_FALSE(cql::Date::create(1970, 1, 1) > cql::Date::create(1970, 1, 1));
		ASSERT_FALSE(cql::Date::create(1970, 1, 1) > cql::Date::create(1970, 1, 2));
		ASSERT_TRUE(cql::Date::create(1970, 1, 2) > cql::Date::create(1970, 1, 1));
		auto now = std::chrono::system_clock::now();
		ASSERT_FALSE(cql::Date(now) > now);
		ASSERT_FALSE(cql::Date(now) > now + std::chrono::seconds(1));
		ASSERT_TRUE(now > cql::Date(now) - std::chrono::seconds(1));
	}
	{
		// greater or equal than
		ASSERT_TRUE(cql::Date::create(1970, 1, 1) >= cql::Date::create(1970, 1, 1));
		ASSERT_FALSE(cql::Date::create(1970, 1, 1) >= cql::Date::create(1970, 1, 2));
		ASSERT_TRUE(cql::Date::create(1970, 1, 2) >= cql::Date::create(1970, 1, 1));
		auto now = std::chrono::system_clock::now();
		ASSERT_TRUE(cql::Date(now) >= now);
		ASSERT_FALSE(cql::Date(now) >= now + std::chrono::seconds(1));
		ASSERT_TRUE(now >= cql::Date(now) - std::chrono::seconds(1));
	}
	{
		// less than
		ASSERT_FALSE(cql::Date::create(1970, 1, 1) < cql::Date::create(1970, 1, 1));
		ASSERT_TRUE(cql::Date::create(1970, 1, 1) < cql::Date::create(1970, 1, 2));
		ASSERT_FALSE(cql::Date::create(1970, 1, 2) < cql::Date::create(1970, 1, 1));
		auto now = std::chrono::system_clock::now();
		ASSERT_FALSE(cql::Date(now) < now);
		ASSERT_TRUE(cql::Date(now) < now + std::chrono::seconds(1));
		ASSERT_FALSE(now < cql::Date(now) - std::chrono::seconds(1));
	}
	{
		// less or equal than
		ASSERT_TRUE(cql::Date::create(1970, 1, 1) <= cql::Date::create(1970, 1, 1));
		ASSERT_TRUE(cql::Date::create(1970, 1, 1) <= cql::Date::create(1970, 1, 2));
		ASSERT_FALSE(cql::Date::create(1970, 1, 2) <= cql::Date::create(1970, 1, 1));
		auto now = std::chrono::system_clock::now();
		ASSERT_TRUE(cql::Date(now) <= now);
		ASSERT_TRUE(cql::Date(now) <= now + std::chrono::seconds(1));
		ASSERT_FALSE(now <= cql::Date(now) - std::chrono::seconds(1));
	}
	{
		// addition assignment
		auto value = cql::Date::create(1970, 1, 1);
		ASSERT_EQ(cql::joinString("", value += std::chrono::hours(48)), "1970-01-03");
		ASSERT_EQ(cql::joinString("", value += std::chrono::hours(48)), "1970-01-05");
	}
	{
		// subtraction assignment
		auto value = cql::Date::create(1970, 1, 1);
		ASSERT_EQ(cql::joinString("", value -= std::chrono::hours(48)), "1969-12-30");
		ASSERT_EQ(cql::joinString("", value -= std::chrono::hours(48)), "1969-12-28");
	}
}

