#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include <LowLevel/CqlConnectionInfo.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlConnectionInfo, defaultVersion) {
	cql::CqlConnectionInfo info;
	ASSERT_EQ(info.getVersion(), 4);
	ASSERT_EQ(info.getHeaderSize(), 9);
	ASSERT_EQ(info.getMaximumMessageBodySize(), 256 * 1024 * 1024);
}

TEST(TestCqlConnectionInfo, useUnSupportedVersion) {
	cql::CqlConnectionInfo info;
	ASSERT_THROWS(cql::CqlNotImplementedException, info.useVersion(1));
}

