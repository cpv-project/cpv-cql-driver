#pragma once
#include <gtest/gtest.h>

#define TEST_FUTURE(caseName, testName) \
	static seastar::future<> caseName##_##testName##_FutureTestBody(); \
	TEST(caseName, testName) { \
		seastar::future<> f(caseName##_##testName##_FutureTestBody());\
		f.get(); \
	} \
	static seastar::future<> caseName##_##testName##_FutureTestBody()

namespace cql {
	namespace Internal_Gtest {
		int runAllTests(int argc, char** argv);
	}
}

