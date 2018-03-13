#include <CQLDriver/Common/ColumnTypes/UUID.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestUUID, getset) {
	cql::UUID uuid;
	ASSERT_TRUE(uuid.empty());

	uuid.set("00112233-4455-6677-8899-aabbccddeeff");
	ASSERT_EQ(uuid.get().first, 0x0011223344556677);
	ASSERT_EQ(uuid.get().second, 0x8899aabbccddeeff);

	uuid = cql::UUID(std::make_pair(0x12345678abcdefaaU, 0x87654321ffabcdefU));
	ASSERT_EQ(uuid.str(), "12345678-ABCD-EFAA-8765-4321FFABCDEF");
}

TEST(TestUUID, encodeBody) {
	{
		cql::UUID value("00112233-4455-6677-8899-aabbccddeeff");
		std::string data;
		value.encodeBody(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff"));
	}
}

TEST(TestUUID, decodeBody) {
	cql::UUID value;
	{
		auto data = makeTestString(
			"\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff");
		value.decodeBody(data.data(), data.size());
		ASSERT_EQ(value.str(), "00112233-4455-6677-8899-AABBCCDDEEFF");
	}
}

TEST(TestUUID, operations) {
	{
		cql::UUID uuid;
		uuid = "00112233-4455-6677-8899-AABBCCDDEEFF";
		cql::UUIDDataType uuidValue = uuid;
		ASSERT_EQ(uuidValue.first, 0x0011223344556677);
		ASSERT_EQ(uuidValue.second, 0x8899aabbccddeeff);
	}
	{
		cql::UUID a("00112233-4455-6677-8899-AABBCCDDEEFF");
		cql::UUID b("00112233-4455-6677-8899-aabbccddeeff");
		cql::UUID c("00112233-4455-6677-8899-aabbccddeefa");
		ASSERT_TRUE(a == b);
		ASSERT_FALSE(a == c);
		ASSERT_FALSE(a != b);
		ASSERT_TRUE(a != c);
	}
	{
		cql::UUID uuid("00112233-4455-6677-8899-aabbccddeeff");
		ASSERT_EQ(cql::joinString("", uuid), "00112233-4455-6677-8899-AABBCCDDEEFF");
	}
}

