#include <CQLDriver/Common/ColumnTypes/TimeUUID.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestTimeUUID, getset) {
	cql::TimeUUID uuid;
	ASSERT_TRUE(uuid.empty());

	uuid.set("00112233-4455-1677-8899-aabbccddeeff");
	ASSERT_EQ(uuid.get().first, 0x0011223344551677);
	ASSERT_EQ(uuid.get().second, 0x8899aabbccddeeff);

	uuid = cql::TimeUUID(std::make_pair(0x12345678abcd1faaU, 0x87654321ffabcdefU));
	ASSERT_EQ(uuid.str(), "12345678-ABCD-1FAA-8765-4321FFABCDEF");
}

TEST(TestTimeUUID, encodeBody) {
	{
		cql::TimeUUID value("00112233-4455-1677-8899-aabbccddeeff");
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x11\x22\x33\x44\x55\x16\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff"));
	}
}

TEST(TestTimeUUID, decodeBody) {
	cql::TimeUUID value;
	{
		auto data = makeTestString(
			"\x00\x11\x22\x33\x44\x55\x16\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.str(), "00112233-4455-1677-8899-AABBCCDDEEFF");
	}
}

TEST(TestTimeUUID, operations) {
	{
		cql::TimeUUID uuid;
		uuid = "00112233-4455-1677-8899-AABBCCDDEEFF";
		cql::UUIDDataType uuidValue = uuid;
		ASSERT_EQ(uuidValue.first, 0x0011223344551677);
		ASSERT_EQ(uuidValue.second, 0x8899aabbccddeeff);
	}
	{
		auto a = cql::TimeUUID::create();
		auto b = a;
		auto c = cql::TimeUUID::create();
		ASSERT_TRUE(b >= a);
		ASSERT_FALSE(b >= c);
		ASSERT_TRUE(c > b);
		ASSERT_FALSE(b > a);
		ASSERT_TRUE(a <= b);
		ASSERT_FALSE(c <= b);
		ASSERT_TRUE(a < c);
		ASSERT_FALSE(a < b);
	}
	{
		cql::TimeUUID uuid("00112233-4455-1677-8899-aabbccddeeff");
		ASSERT_EQ(cql::joinString("", uuid), "00112233-4455-1677-8899-AABBCCDDEEFF");
	}
}

