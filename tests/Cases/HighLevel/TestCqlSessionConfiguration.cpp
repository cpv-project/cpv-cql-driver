#include <CqlDriver/HighLevel/CqlSessionConfiguration.hpp>
#include <gtest/gtest.h>

TEST(TestSessionConfiguration, properties) {
	cql::CqlSessionConfiguration configuration;

	// get default values
	ASSERT_TRUE(configuration.getServerAddresses().empty());
	ASSERT_FALSE(configuration.getConnectWithSsl());
	ASSERT_TRUE(configuration.getUsername().empty());
	ASSERT_TRUE(configuration.getPassword().empty());
	ASSERT_EQ(configuration.getMaxPoolSize(), 100U);
	ASSERT_FALSE(configuration.getAutoDiscoverServerAddresses());

	// set new values
	configuration.addServerAddress("127.0.0.1");
	configuration.addServerAddress("123.100.101.123", 1234);
	configuration.setConnectWithSsl(true);
	configuration.setUsername("some username");
	configuration.setPassword("some password");
	configuration.setMaxPoolSize(101);
	configuration.setAutoDiscoverServerAddress(true);

	// get new values
	ASSERT_EQ(configuration.getServerAddresses().size(), 2U);
	auto& addresses = configuration.getServerAddresses();
	ASSERT_EQ(addresses.at(0).u.in.sin_family, AF_INET);
	ASSERT_EQ(addresses.at(0).u.in.sin_port, htons(9042));
	ASSERT_EQ(addresses.at(0).u.in.sin_addr.s_addr, htonl(0x7f000001));
	ASSERT_EQ(addresses.at(1).u.in.sin_family, AF_INET);
	ASSERT_EQ(addresses.at(1).u.in.sin_port, htons(1234));
	ASSERT_EQ(addresses.at(1).u.in.sin_addr.s_addr, htonl(0x7b64657b));
	ASSERT_TRUE(configuration.getConnectWithSsl());
	ASSERT_EQ(configuration.getUsername(), "some username");
	ASSERT_EQ(configuration.getPassword(), "some password");
	ASSERT_EQ(configuration.getMaxPoolSize(), 101U);
	ASSERT_TRUE(configuration.getAutoDiscoverServerAddresses());
}

