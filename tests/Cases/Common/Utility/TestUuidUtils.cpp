#include <CqlDriver/Common/Utility/UuidUtils.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestUuidUtils, strToUuid) {
	cql::UuidDataType uuid = cql::strToUuid("00112233-4455-6677-8899-aabbccddeeff");
	ASSERT_EQ(uuid.first, 0x0011223344556677);
	ASSERT_EQ(uuid.second, 0x8899aabbccddeeff);
}

TEST(TestUuidUtils, uuidToStr) {
	cql::UuidDataType uuid(0x12345678abcdefaaU, 0x87654321ffabcdefU);
	ASSERT_EQ(cql::uuidToStr(uuid), "12345678-ABCD-EFAA-8765-4321FFABCDEF");
}

TEST(TestUuidUtils, makeEmptyUuid) {
	cql::UuidDataType emptyUuid = cql::makeEmptyUuid();
	ASSERT_EQ(cql::uuidToStr(emptyUuid), "00000000-0000-0000-0000-000000000000");
	ASSERT_EQ(emptyUuid, cql::makeEmptyUuid());
}

TEST(TestUuidUtils, makeRandomUuid) {
	cql::UuidDataType a = cql::makeRandomUuid();
	cql::UuidDataType b = cql::makeRandomUuid();
	ASSERT_FALSE(a == b);
	ASSERT_EQ(a.first & 0xf000, 0x4000); // version
	ASSERT_EQ(a.second & 0xc000'0000'0000'0000, 0x8000'0000'0000'0000); // variant
}

TEST(TestUuidUtils, makeTimeUuid) {
	cql::UuidDataType a = cql::makeTimeUuid();
	cql::UuidDataType b = cql::makeTimeUuid();
	ASSERT_EQ(cql::getVersionFromUuid(a), cql::TimeUuidVersion);
	ASSERT_TRUE(a < b);
}

TEST(TestUuidUtils, makeMinTimeUuid) {
	cql::UuidDataType uuid = cql::makeTimeUuid();
	cql::UuidDataType minUuid = cql::makeMinTimeUuid(cql::getTimeFromUuid(uuid));
	ASSERT_GE(uuid.first, minUuid.first);
	ASSERT_GE(uuid.second, minUuid.second);
	ASSERT_EQ(minUuid.second, 0x8000'0000'0000'0000ULL);
}

TEST(TestUuidUtils, makeMaxTimeUuid) {
	cql::UuidDataType uuid = cql::makeTimeUuid();
	cql::UuidDataType maxUuid = cql::makeMaxTimeUuid(cql::getTimeFromUuid(uuid));
	ASSERT_LE(uuid.first, maxUuid.first);
	ASSERT_LE(uuid.second, maxUuid.second);
	ASSERT_EQ(maxUuid.second, 0xbfff'ffff'ffff'ffffULL);
}

TEST(TestUuidUtils, getVersionFromUuid) {
	ASSERT_EQ(cql::getVersionFromUuid(cql::makeEmptyUuid()), cql::EmptyUuidVersion);
	ASSERT_EQ(cql::getVersionFromUuid(cql::makeRandomUuid()), cql::RandomUuidVersion);
	ASSERT_EQ(cql::getVersionFromUuid(cql::makeTimeUuid()), cql::TimeUuidVersion);
	ASSERT_EQ(cql::getVersionFromUuid(cql::strToUuid("00112233-4455-6677-8899-aabbccddeeff")), 6);
}

TEST(TestUuidUtils, getTimeFromUuid) {
	cql::UuidDataType uuid = cql::makeTimeUuid();
	auto time = cql::getTimeFromUuid(uuid);
	cql::UuidDataType minUuid = cql::makeMinTimeUuid(time);
	ASSERT_EQ(cql::getTimeFromUuid(minUuid), time);
}

