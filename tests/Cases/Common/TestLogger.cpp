#include <CQLDriver/Common/Logger.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestLogger, logLevel) {
	auto logger = cql::Logger::createConsole(cql::LogLevel::Warning);
	ASSERT_EQ(logger->getLogLevel(), cql::LogLevel::Warning);
	ASSERT_TRUE(logger->isEnabled(cql::LogLevel::Error));
	ASSERT_TRUE(logger->isEnabled(cql::LogLevel::Warning));
	ASSERT_FALSE(logger->isEnabled(cql::LogLevel::Notice));

	logger->setLogLevel(cql::LogLevel::Info);
	ASSERT_EQ(logger->getLogLevel(), cql::LogLevel::Info);
	ASSERT_TRUE(logger->isEnabled(cql::LogLevel::Notice));
	ASSERT_TRUE(logger->isEnabled(cql::LogLevel::Info));
	ASSERT_FALSE(logger->isEnabled(cql::LogLevel::Debug));
}

TEST(TestLogger, create) {
	{
		auto logger = cql::Logger::createConsole(cql::LogLevel::Warning);
		ASSERT_TRUE(logger != nullptr);
	}
	{
		auto logger = cql::Logger::createNoop();
		ASSERT_TRUE(logger != nullptr);
	}
}

