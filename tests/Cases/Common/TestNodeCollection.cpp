#include <CQLDriver/Common/NodeCollection.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestNodeCollection, create) {
	auto collection = cql::NodeCollection::create({
		cql::NodeConfiguration().setAddress("a", 1024),
		cql::NodeConfiguration().setAddress("b", 1024),
		cql::NodeConfiguration().setAddress("c", 1024)
	});
	ASSERT_TRUE(collection.get() != nullptr);
}

