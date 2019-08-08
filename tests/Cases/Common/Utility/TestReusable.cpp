#include <CQLDriver/Common/Utility/Reusable.hpp>
#include <TestUtility/GTestUtils.hpp>
#include <seastar/core/shared_ptr.hh>

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
	struct C : A, B {
		int c;
		static void freeResources() { }
		static void reset() { }
	};
}

template <>
thread_local cql::ReusableStorageType<Foo> cql::ReusableStorageInstance<Foo>;

template <>
thread_local cql::ReusableStorageType<Derived> cql::ReusableStorageInstance<Derived>;

template <>
thread_local cql::ReusableStorageType<C> cql::ReusableStorageInstance<C>;

TEST(TestReusable, Simple) {
	for (std::size_t i = 0; i < 3; ++i) {
		auto foo = cql::makeReusable<Foo>();
		ASSERT_TRUE(foo.get());
		ASSERT_TRUE(foo->isFree);
		ASSERT_TRUE(foo->isReset);
		foo->isFree = false;
		foo->isReset = false;
	}
}

TEST(TestReusable, UpCasting) {
	auto record = seastar::make_shared<int>(0);
	for (int i = 0; i < 3; ++i) {
		ASSERT_EQ(*record, i);
		{
			auto base = cql::makeReusable<Derived>(record).cast<Base>();
		}
		ASSERT_EQ(*record, i+1);
	}
}

TEST(TestReusable, DownCasting) {
	auto record = seastar::make_shared<int>(0);
	for (int i = 0; i < 3; ++i) {
		ASSERT_EQ(*record, i);
		{
			auto base = cql::makeReusable<Derived>(record).cast<Base>();
			auto derived = std::move(base).cast<Derived>();
		}
		ASSERT_EQ(*record, i+1);
	}
}

TEST(TestReusable, InvalidCasting) {
	cql::makeReusable<C>().cast<A>();
	ASSERT_THROWS_CONTAINS(
		cql::LogicException,
		cql::makeReusable<C>().cast<B>(),
		"cast cause pointer address changed");
}

TEST(TestReusable, moveAssignment) {
	auto record = seastar::make_shared<int>(0);
	for (int i = 0; i < 3; ++i) {
		ASSERT_EQ(*record, i);
		{
			auto a = cql::makeReusable<Derived>(record);
			auto b = cql::Reusable<Derived>(nullptr).cast<Base>();
			cql::Reusable<Derived> c(nullptr);
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

