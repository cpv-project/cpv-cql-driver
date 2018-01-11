#include <CqlDriver/Common/CqlCommonDefinitions.hpp>
#include <CqlDriver/Common/CqlNodeConfiguration.hpp>
#include <CqlDriver/Common/Utility/StringUtils.hpp>
#include <TestUtility/GTestUtils.hpp>
#include <thread>

TEST(TestCqlNodeConfiguration, all) {
	{
		cql::CqlNodeConfiguration configuration;
		ASSERT_TRUE(configuration.getAddress().first.empty());
		ASSERT_EQ(configuration.getAddress().second, 0);
		ASSERT_FALSE(configuration.getUseSsl());
		ASSERT_FALSE(configuration.getUseCompression());
		ASSERT_EQ(configuration.getAuthenticatorClass(), cql::CqlAuthenticatorClasses::AllowAllAuthenticator);
		ASSERT_TRUE(configuration.getAuthenticatorData().empty());
		seastar::socket_address address;
		ASSERT_FALSE(configuration.getIpAddress(address, std::chrono::milliseconds(1000)));
	}
	{
		auto configuration = cql::CqlNodeConfiguration()
			.setAddress("127.0.0.1", 9000)
			.setUseSsl(true)
			.setUseCompression(true)
			.setPasswordAuthentication("abc", "asdfg");
		ASSERT_EQ(configuration.getAddress().first, "127.0.0.1");
		ASSERT_EQ(configuration.getAddress().second, 9000);
		ASSERT_TRUE(configuration.getUseSsl());
		ASSERT_TRUE(configuration.getUseCompression());
		ASSERT_EQ(configuration.getAuthenticatorClass(), cql::CqlAuthenticatorClasses::PasswordAuthenticator);
		ASSERT_EQ(configuration.getAuthenticatorData(), seastar::sstring("abc""\x00""asdfg", 9));
		seastar::socket_address address;
		ASSERT_TRUE(configuration.getIpAddress(address, std::chrono::milliseconds(1000)));
		ASSERT_EQ(cql::joinString("", address), "127.0.0.1:9000");
	}
	{
		auto configuration = cql::CqlNodeConfiguration()
			.setAddress("localhost", 9000)
			.setUseSsl(true)
			.setUseCompression(true)
			.setPasswordAuthentication("abc", "asdfg");
		seastar::socket_address address;
		ASSERT_FALSE(configuration.getIpAddress(address, std::chrono::milliseconds(1000)));
		configuration.updateIpAddress(seastar::socket_address(seastar::ipv4_addr("127.0.0.1", 9000)));
		ASSERT_TRUE(configuration.getIpAddress(address, std::chrono::milliseconds(1000)));
		ASSERT_EQ(cql::joinString("", address), "127.0.0.1:9000");
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
		ASSERT_FALSE(configuration.getIpAddress(address, std::chrono::milliseconds(1)));
	}
}

