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

	struct A { int a; };
	struct B { int b; };
	struct C : A, B { int c; void freeResources() { } void reset() { } };
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

TEST(TestCqlObject, UpCasting) {
	auto record = seastar::make_shared<int>(0);
	for (std::size_t i = 0; i < 3; ++i) {
		ASSERT_EQ(*record, i);
		{
			auto base = cql::makeObject<Derived>(record).cast<Base>();
		}
		ASSERT_EQ(*record, i+1);
	}
}

TEST(TestCqlObject, DownCasting) {
	auto record = seastar::make_shared<int>(0);
	for (std::size_t i = 0; i < 3; ++i) {
		ASSERT_EQ(*record, i);
		{
			auto base = cql::makeObject<Derived>(record).cast<Base>();
			auto derived = std::move(base).cast<Derived>();
		}
		ASSERT_EQ(*record, i+1);
	}
}

TEST(TestCqlObject, InvalidCasting) {
	cql::makeObject<C>().cast<A>();
	ASSERT_THROWS_CONTAINS(
		cql::CqlLogicException,
		cql::makeObject<C>().cast<B>(),
		"cast cause pointer address changed");
}

TEST(TestCqlObject, moveAssignment) {
	auto record = seastar::make_shared<int>(0);
	for (std::size_t i = 0; i < 3; ++i) {
		ASSERT_EQ(*record, i);
		{
			auto a = cql::makeObject<Derived>(record);
			auto b = cql::CqlObject<Derived>(nullptr).cast<Base>();
			cql::CqlObject<Derived> c(nullptr);
			b = std::move(a).cast<Base>();
			b = std::move(b);
			c = std::move(b).cast<Derived>();
			c = std::move(c);
			ASSERT_TRUE(a == nullptr);
			ASSERT_TRUE(b == nullptr);
			ASSERT_TRUE(c != nullptr);
		}
		ASSERT_EQ(*record, i+1);
	}
}

