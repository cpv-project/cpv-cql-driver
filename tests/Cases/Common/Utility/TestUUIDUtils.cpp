#include <CQLDriver/Common/Utility/UUIDUtils.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestUUIDUtils, strToUUID) {
	cql::UUIDDataType uuid = cql::strToUUID("00112233-4455-6677-8899-aabbccddeeff");
	ASSERT_EQ(uuid.first, 0x0011223344556677);
	ASSERT_EQ(uuid.second, 0x8899aabbccddeeff);
}

TEST(TestUUIDUtils, uuidToStr) {
	cql::UUIDDataType uuid(0x12345678abcdefaaU, 0x87654321ffabcdefU);
	ASSERT_EQ(cql::uuidToStr(uuid), "12345678-ABCD-EFAA-8765-4321FFABCDEF");
}

TEST(TestUUIDUtils, makeEmptyUUID) {
	cql::UUIDDataType emptyUUID = cql::makeEmptyUUID();
	ASSERT_EQ(cql::uuidToStr(emptyUUID), "00000000-0000-0000-0000-000000000000");
	ASSERT_EQ(emptyUUID, cql::makeEmptyUUID());
}

TEST(TestUUIDUtils, makeRandomUUID) {
	cql::UUIDDataType a = cql::makeRandomUUID();
	cql::UUIDDataType b = cql::makeRandomUUID();
	ASSERT_FALSE(a == b);
	ASSERT_EQ(a.first & 0xf000, 0x4000); // version
	ASSERT_EQ(a.second & 0xc000'0000'0000'0000, 0x8000'0000'0000'0000); // variant
}

TEST(TestUUIDUtils, makeTimeUUID) {
	cql::UUIDDataType a = cql::makeTimeUUID();
	cql::UUIDDataType b = cql::makeTimeUUID();
	ASSERT_EQ(cql::getVersionFromUUID(a), cql::TimeUUIDVersion);
	ASSERT_TRUE(a < b);
}

TEST(TestUUIDUtils, makeMinTimeUUID) {
	cql::UUIDDataType uuid = cql::makeTimeUUID();
	cql::UUIDDataType minUUID = cql::makeMinTimeUUID(cql::getTimeFromUUID(uuid));
	ASSERT_GE(uuid.first, minUUID.first);
	ASSERT_GE(uuid.second, minUUID.second);
	ASSERT_EQ(minUUID.second, 0x8000'0000'0000'0000ULL);
}

TEST(TestUUIDUtils, makeMaxTimeUUID) {
	cql::UUIDDataType uuid = cql::makeTimeUUID();
	cql::UUIDDataType maxUUID = cql::makeMaxTimeUUID(cql::getTimeFromUUID(uuid));
	ASSERT_LE(uuid.first, maxUUID.first);
	ASSERT_LE(uuid.second, maxUUID.second);
	ASSERT_EQ(maxUUID.second, 0xbfff'ffff'ffff'ffffULL);
}

TEST(TestUUIDUtils, getVersionFromUUID) {
	ASSERT_EQ(cql::getVersionFromUUID(cql::makeEmptyUUID()), cql::EmptyUUIDVersion);
	ASSERT_EQ(cql::getVersionFromUUID(cql::makeRandomUUID()), cql::RandomUUIDVersion);
	ASSERT_EQ(cql::getVersionFromUUID(cql::makeTimeUUID()), cql::TimeUUIDVersion);
	ASSERT_EQ(cql::getVersionFromUUID(cql::strToUUID("00112233-4455-6677-8899-aabbccddeeff")), 6);
}

TEST(TestUUIDUtils, getTimeFromUUID) {
	cql::UUIDDataType uuid = cql::makeTimeUUID();
	auto time = cql::getTimeFromUUID(uuid);
	cql::UUIDDataType minUUID = cql::makeMinTimeUUID(time);
	ASSERT_EQ(cql::getTimeFromUUID(minUUID), time);
}

