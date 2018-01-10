#include <CqlDriver/Common/CqlSessionConfiguration.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestSessionConfiguration, all) {
	cql::CqlSessionConfiguration configuration;
	ASSERT_EQ(configuration.getMaxPoolSize(), 100);
	ASSERT_EQ(configuration.getDnsCacheTime().count(), 30000);
	configuration = cql::CqlSessionConfiguration()
		.setMaxPoolSize(101)
		.setDnsCacheTime(std::chrono::milliseconds(30001));	
	ASSERT_EQ(configuration.getMaxPoolSize(), 101);
	ASSERT_EQ(configuration.getDnsCacheTime().count(), 30001);
}

