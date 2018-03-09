#include <CQLDriver/Common/ColumnTypes/Timestamp.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestTimestamp, getset) {
	cql::Timestamp value;
	auto now = std::chrono::system_clock::now();
	value.set(now);
	ASSERT_EQ(value.get(), now);

	auto nowTime = std::time(nullptr);
	value.set(nowTime);
	ASSERT_EQ(static_cast<std::time_t>(value), nowTime);

	value.reset();
	ASSERT_EQ(static_cast<std::chrono::milliseconds>(value).count(), 0);

	std::tm nowTm = {};
	::localtime_r(&nowTime, &nowTm);
	value.set(nowTm);
	std::tm getTm = value;
	ASSERT_EQ(std::mktime(&getTm), std::mktime(&nowTm));

	value = cql::Timestamp();
	ASSERT_EQ(static_cast<std::chrono::milliseconds>(value).count(), 0);
}

TEST(TestTimestamp, encodeBody) {
	{
		cql::Timestamp value;
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00\x00\x00\x00\x00"));
	}
	{
		cql::Timestamp value(std::chrono::seconds(90123));
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x00\x05\x5f\x2a\xf8"));
	}
	{
		cql::Timestamp value(std::chrono::seconds(-60));
		seastar::sstring data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString("\xff\xff\xff\xff\xff\xff\x15\xa0"));
	}
}

TEST(TestTimestamp, decodeBody) {
	cql::Timestamp value;
	{
		auto data = makeTestString("\x00\x00\x00\x00\x00\x00\x00\x00");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(static_cast<std::chrono::milliseconds>(value).count(), 0);
	}
	{
		auto data = makeTestString("\x00\x00\x00\x00\x05\x5f\x2a\xf8");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(static_cast<std::chrono::milliseconds>(value).count(), 90'123'000);
	}
	{
		auto data = makeTestString("\xff\xff\xff\xff\xff\xff\x15\xa0");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(static_cast<std::chrono::milliseconds>(value).count(), -60'000);
	}
}

TEST(TestTimestamp, operations) {
	{
		// assign and cast
		cql::Timestamp value;
		value = std::chrono::system_clock::now();
		cql::Timestamp::UnderlyingType timeStampValue = value;
		ASSERT_EQ(timeStampValue, value.get());
	}
	{
		// addition
		auto value = cql::Timestamp::create(1970, 1, 1, 0, 0, 0);
		auto minuteAfter = value + std::chrono::minutes(1);
		auto hourBefore = value + std::chrono::hours(-1);
		ASSERT_EQ(cql::joinString("", value), "1970-01-01 00:00:00.000");
		ASSERT_EQ(cql::joinString("", minuteAfter), "1970-01-01 00:01:00.000");
		ASSERT_EQ(cql::joinString("", hourBefore), "1969-12-31 23:00:00.000");
	}
	{
		// subtraction
		auto value = cql::Timestamp::create(1970, 1, 2, 1, 2, 3);
		auto dayBefore = value - std::chrono::hours(24);
		ASSERT_EQ(cql::joinString("", dayBefore), "1970-01-01 01:02:03.000");
	}
	{
		// difference
		auto a = cql::Timestamp::create(1970, 1, 1, 1, 2, 3);
		auto b = cql::Timestamp::create(1970, 1, 1, 1, 0, 3);
		ASSERT_EQ(std::chrono::duration_cast<std::chrono::milliseconds>(a - b).count(), 120'000);
		ASSERT_EQ(std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count(), -120'000);
	}
	{
		// equal to
		ASSERT_TRUE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) ==
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0));
		ASSERT_FALSE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) ==
			cql::Timestamp::create(1970, 1, 1, 0, 0, 1));
	}
	{
		// not equal to
		ASSERT_FALSE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) !=
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0));
		ASSERT_TRUE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) !=
			cql::Timestamp::create(1970, 1, 1, 0, 0, 1));
	}
	{
		// greater than
		ASSERT_FALSE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) >
			cql::Timestamp::create(1970, 1, 1, 0, 0, 1));
		ASSERT_FALSE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) >
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0));
		ASSERT_TRUE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) >
			cql::Timestamp::create(1969, 12, 31, 12, 59, 59));
	}
	{
		// greater or equal than
		ASSERT_FALSE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) >=
			cql::Timestamp::create(1970, 1, 1, 0, 0, 1));
		ASSERT_TRUE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) >=
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0));
		ASSERT_TRUE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) >=
			cql::Timestamp::create(1969, 12, 31, 12, 59, 59));
	}
	{
		// less than
		ASSERT_TRUE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) <
			cql::Timestamp::create(1970, 1, 1, 0, 0, 1));
		ASSERT_FALSE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) <
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0));
		ASSERT_FALSE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) <
			cql::Timestamp::create(1969, 12, 31, 12, 59, 59));
	}
	{
		// less or equal than
		ASSERT_TRUE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) <=
			cql::Timestamp::create(1970, 1, 1, 0, 0, 1));
		ASSERT_TRUE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) <=
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0));
		ASSERT_FALSE(
			cql::Timestamp::create(1970, 1, 1, 0, 0, 0) <=
			cql::Timestamp::create(1969, 12, 31, 12, 59, 59));
	}
	{
		// addition assignment
		auto value = cql::Timestamp::create(1970, 1, 1, 0, 0, 0);
		ASSERT_EQ(cql::joinString("", value += std::chrono::hours(48)), "1970-01-03 00:00:00.000");
		ASSERT_EQ(cql::joinString("", value += std::chrono::minutes(48)), "1970-01-03 00:48:00.000");
	}
	{
		// subtraction assignment
		auto value = cql::Timestamp::create(1970, 1, 1, 0, 0, 0);
		ASSERT_EQ(cql::joinString("", value -= std::chrono::hours(48)), "1969-12-30 00:00:00.000");
		ASSERT_EQ(cql::joinString("", value -= std::chrono::minutes(48)), "1969-12-29 23:12:00.000");
	}
}

