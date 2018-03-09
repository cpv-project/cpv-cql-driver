#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>
#include <LowLevel/ConnectionInfo.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestConnectionInfo, defaultVersion) {
	cql::ConnectionInfo info;
	ASSERT_EQ(info.getVersion(), 4);
	ASSERT_EQ(info.getHeaderSize(), 9);
	ASSERT_EQ(info.getMaximumMessageBodySize(), 256 * 1024 * 1024);
}

TEST(TestConnectionInfo, useUnSupportedVersion) {
	cql::ConnectionInfo info;
	ASSERT_THROWS(cql::NotImplementedException, info.useVersion(1));
}

