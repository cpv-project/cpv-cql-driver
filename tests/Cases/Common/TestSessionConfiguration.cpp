#include <CQLDriver/Common/SessionConfiguration.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestSessionConfiguration, all) {
	cql::SessionConfiguration configuration;
	ASSERT_EQ(configuration.getMaxPoolSize(), 100);
	ASSERT_EQ(configuration.getDnsCacheTime().count(), 30000);
	configuration = cql::SessionConfiguration()
		.setMinPoolSize(7)
		.setMaxPoolSize(101)
		.setMaxWaitersAfterConnectionsExhausted(102)
		.setDnsCacheTime(std::chrono::milliseconds(30001))
		.setDefaultKeySpace("abc");
	ASSERT_EQ(configuration.getMinPoolSize(), 7);
	ASSERT_EQ(configuration.getMaxPoolSize(), 101);
	ASSERT_EQ(configuration.getMaxWaitersAfterConnectionsExhausted(), 102);
	ASSERT_EQ(configuration.getDnsCacheTime().count(), 30001);
	ASSERT_EQ(configuration.getDefaultKeySpace(), "abc");
}

