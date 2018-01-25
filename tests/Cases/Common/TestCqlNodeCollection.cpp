#include <CqlDriver/Common/CqlNodeCollection.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlNodeCollection, create) {
	auto collection = cql::CqlNodeCollection::create({
		cql::CqlNodeConfiguration().setAddress("a", 1024),
		cql::CqlNodeConfiguration().setAddress("b", 1024),
		cql::CqlNodeConfiguration().setAddress("c", 1024)
	});
	ASSERT_TRUE(collection.get() != nullptr);
}

