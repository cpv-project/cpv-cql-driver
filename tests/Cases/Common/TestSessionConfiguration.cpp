#include <CQLDriver/Common/SessionConfiguration.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestSessionConfiguration, all) {
	cql::SessionConfiguration configuration;
	ASSERT_EQ(configuration.getMaxPoolSize(), 100);
	ASSERT_EQ(configuration.getDnsCacheTime().count(), 30000);
	ASSERT_EQ(configuration.getMaxWaitersAfterConnectionsExhausted(), 100);
	ASSERT_EQ(configuration.getDnsCacheTime().count(), 30000);
	ASSERT_EQ(configuration.getDefaultKeySpace(), "");
	ASSERT_EQ(configuration.getDefaultConsistency(), cql::ConsistencyLevel::Quorum);
	ASSERT_FALSE(configuration.getPrepareAllQueries());
	configuration = cql::SessionConfiguration()
		.setMinPoolSize(7)
		.setMaxPoolSize(101)
		.setMaxWaitersAfterConnectionsExhausted(102)
		.setDnsCacheTime(std::chrono::milliseconds(30001))
		.setDefaultKeySpace("abc")
		.setPrepareAllQueries(true)
		.setDefaultConsistency(cql::ConsistencyLevel::LocalOne);
	ASSERT_EQ(configuration.getMinPoolSize(), 7);
	ASSERT_EQ(configuration.getMaxPoolSize(), 101);
	ASSERT_EQ(configuration.getMaxWaitersAfterConnectionsExhausted(), 102);
	ASSERT_EQ(configuration.getDnsCacheTime().count(), 30001);
	ASSERT_EQ(configuration.getDefaultKeySpace(), "abc");
	ASSERT_EQ(configuration.getDefaultConsistency(), cql::ConsistencyLevel::LocalOne);
	ASSERT_TRUE(configuration.getPrepareAllQueries());
}

