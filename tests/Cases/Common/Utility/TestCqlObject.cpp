#include <CqlDriver/Common/Utility/CqlObject.hpp>
#include <TestUtility/GTestUtils.hpp>
#include <core/shared_ptr.hh>

namespace {
	struct Foo {
		bool isFree = true;
		bool isReset = false;
		void freeResources() { isFree = true; }
		void reset() { isReset = true; }
	};

	struct Base { };

	struct Derived : public Base {
		seastar::shared_ptr<int> record_;
		void freeResources() { ++*record_; record_ = nullptr; }
		void reset(const seastar::shared_ptr<int>& record) { record_ = record; }
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

TEST(TestCqlObject, Polymorphism) {
	auto record = seastar::make_shared<int>(0);
	for (std::size_t i = 0; i < 3; ++i) {
		ASSERT_EQ(*record, i);
		{
			cql::CqlObject<Base> base(cql::makeObject<Derived>(record));
		}
		ASSERT_EQ(*record, i+1);
	}
}

