#include <CqlDriver/Utility/CqlObject.hpp>
#include <TestUtility/GTestUtils.hpp>

namespace {
	struct Foo {
		bool isFree = true;
		bool isReset = false;
		void freeResources() { isFree = true; }
		void reset() { isReset = true; }
	};
}

TEST(TestCqlObject, Simple) {
	for (std::size_t i = 0; i < 3; ++i) {
		auto foo = cql::makeObject<Foo>();
		ASSERT_TRUE(foo.get());
		ASSERT_TRUE(foo->isFree);
		ASSERT_TRUE(foo->isReset);
		foo->isFree = false;
		foo->isReset = false;
	}
}

