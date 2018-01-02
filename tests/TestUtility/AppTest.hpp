#pragma once
#include <CqlDriver/Utility/StringUtils.hpp>
#include <core/app-template.hh>
#include <utility>
#include <gtest/gtest.h>

namespace cql {
	namespace Internal_AppTest { extern char** ProgramName; }

	template <class Func>
	void runAppTest(Func&& func) {
		seastar::app_template app;
		app.run(1, Internal_AppTest::ProgramName, [&func] {
			return func().handle_exception([] (std::exception_ptr e) {
				auto str = cql::joinString("", "exception from AppTest: ", e);
				ASSERT_EQ(str, "");
			});
		});
	}
}

