#include <CqlDriver/HighLevel/CqlSessionConfiguration.hpp>
#include <HighLevel/CqlSessionConfigurationData.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestSessionConfiguration, properties) {
	cql::CqlSessionConfiguration configuration;

	// get default values
	auto& data = configuration.getData();
	ASSERT_TRUE(data->serverAddresses.empty());
	ASSERT_EQ(data->maxPoolSize, 100U);
	ASSERT_FALSE(data->autoDiscoverServerAddress);

	// set new values
	configuration.addServerAddress("127.0.0.1");
	configuration.addServerAddress("123.100.101.123", 1234);
	configuration.connectWithSsl();
	configuration.authenticateWithPassword("some username", "some password");
	configuration.setMaxPoolSize(101);
	configuration.setAutoDiscoverServerAddress(true);

	// get new values
	auto& addresses = data->serverAddresses;
	ASSERT_EQ(addresses.size(), 2U);
	ASSERT_EQ(addresses.at(0).u.in.sin_family, AF_INET);
	ASSERT_EQ(addresses.at(0).u.in.sin_port, htons(9042));
	ASSERT_EQ(addresses.at(0).u.in.sin_addr.s_addr, htonl(0x7f000001));
	ASSERT_EQ(addresses.at(1).u.in.sin_family, AF_INET);
	ASSERT_EQ(addresses.at(1).u.in.sin_port, htons(1234));
	ASSERT_EQ(addresses.at(1).u.in.sin_addr.s_addr, htonl(0x7b64657b));
	ASSERT_TRUE(data->connector);
	ASSERT_TRUE(data->authenticator);
	ASSERT_EQ(data->maxPoolSize, 101U);
	ASSERT_TRUE(data->autoDiscoverServerAddress);
}

