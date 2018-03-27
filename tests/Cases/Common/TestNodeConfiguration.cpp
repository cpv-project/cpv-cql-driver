#include <CQLDriver/Common/CommonDefinitions.hpp>
#include <CQLDriver/Common/NodeConfiguration.hpp>
#include <CQLDriver/Common/Utility/StringUtils.hpp>
#include <TestUtility/GTestUtils.hpp>
#include <thread>

TEST(TestNodeConfiguration, all) {
	{
		cql::NodeConfiguration configuration;
		ASSERT_TRUE(configuration.getAddress().first.empty());
		ASSERT_EQ(configuration.getAddress().second, 0);
		ASSERT_FALSE(configuration.getUseSSL());
		ASSERT_FALSE(configuration.getUseCompression());
		ASSERT_EQ(configuration.getMaxStreams(), 20);
		ASSERT_EQ(configuration.getMaxPendingMessages(), 100);
		ASSERT_EQ(configuration.getAuthenticatorClass(), cql::AuthenticatorClasses::AllowAllAuthenticator);
		ASSERT_TRUE(configuration.getAuthenticatorData().empty());
		seastar::socket_address address;
		ASSERT_FALSE(configuration.getIpAddress(address, std::chrono::milliseconds(1000)));
	}
	{
		auto configuration = cql::NodeConfiguration()
			.setAddress("127.0.0.1", 9000)
			.setUseSSL(true)
			.setUseCompression(true)
			.setMaxStreams(21)
			.setMaxPendingMessages(22)
			.setPasswordAuthentication("abc", "asdfg");
		ASSERT_EQ(configuration.getAddress().first, "127.0.0.1");
		ASSERT_EQ(configuration.getAddress().second, 9000);
		ASSERT_TRUE(configuration.getUseSSL());
		ASSERT_TRUE(configuration.getUseCompression());
		ASSERT_EQ(configuration.getMaxStreams(), 21);
		ASSERT_EQ(configuration.getMaxPendingMessages(), 22);
		ASSERT_EQ(configuration.getAuthenticatorClass(), cql::AuthenticatorClasses::PasswordAuthenticator);
		ASSERT_EQ(configuration.getAuthenticatorData(), makeTestString("\x00""abc""\x00""asdfg"));
		seastar::socket_address address;
		ASSERT_TRUE(configuration.getIpAddress(address, std::chrono::milliseconds(1000)));
		ASSERT_EQ(cql::joinString("", address), "127.0.0.1:9000");
	}
	{
		auto configuration = cql::NodeConfiguration()
			.setAddress("localhost", 9000)
			.setUseSSL(true)
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

