#include <CqlDriver/Exceptions/CqlException.hpp>
#include <gtest/gtest.h>

TEST(TestCqlException, construct) {
	cql::CqlException exception(CQL_CODEINFO, "some error");
	std::string error = exception.what();
	// std::cout << error << std::endl;
	ASSERT_TRUE(error.find("some error") != std::string::npos);
	ASSERT_TRUE(error.find("TestCqlException.cpp") != std::string::npos);
}

