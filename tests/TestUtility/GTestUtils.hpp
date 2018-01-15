#pragma once
#include <gtest/gtest.h>
#include <core/sstring.hh>

#define TEST_FUTURE(caseName, testName) \
	static seastar::future<> caseName##_##testName##_FutureTestBody(); \
	TEST(caseName, testName) { \
		seastar::future<> f(caseName##_##testName##_FutureTestBody());\
		f.get(); \
	} \
	static seastar::future<> caseName##_##testName##_FutureTestBody()

#define ASSERT_THROWS(exception, expression) \
	do { \
		try { expression; } \
		catch (const exception&) { break; } \
		catch (...) { throw; } \
		ASSERT_FALSE("No exception throws"); \
	} while (0)

namespace cql {
	namespace Internal_Gtest {
		int runAllTests(int argc, char** argv);
	}
}

namespace {
	/** Construct string with fixed size, string may contains \x00 */
	template <std::size_t Size>
	seastar::sstring makeTestString(const char(&str)[Size]) {
		static_assert(Size > 0, "string must be null terminated");
		return seastar::sstring(str, Size - 1);
	}
}

