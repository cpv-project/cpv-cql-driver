#include <Common/NodeCollectionImpl.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestNodeCollectionImpl, construct) {
	cql::NodeCollectionImpl nodeCollection({
		cql::NodeConfiguration().setAddress("a", 1024),
		cql::NodeConfiguration().setAddress("b", 1024),
		cql::NodeConfiguration().setAddress("c", 1024)
	});
	ASSERT_EQ(nodeCollection.getNodesCount(), 3U);
}

TEST(TestNodeCollectionImpl, chooseOneNodeSimple) {
	cql::NodeCollectionImpl nodeCollection({
		cql::NodeConfiguration().setAddress("a", 1024),
		cql::NodeConfiguration().setAddress("b", 1024),
		cql::NodeConfiguration().setAddress("c", 1024)
	});
	for (std::size_t i = 0; i < 3; ++i) {
		auto nodeA = nodeCollection.chooseOneNode(); // may not "a"
		auto nodeB = nodeCollection.chooseOneNode(); // may not "b"
		auto nodeC = nodeCollection.chooseOneNode(); // may not "c"
		ASSERT_TRUE(nodeA != nodeB && nodeA != nodeC && nodeB != nodeC);
	}
}

TEST(TestNodeCollectionImpl, chooseOneNodeWithSomeIsFault) {
	cql::NodeCollectionImpl nodeCollection({
		cql::NodeConfiguration().setAddress("a", 1024),
		cql::NodeConfiguration().setAddress("b", 1024),
		cql::NodeConfiguration().setAddress("c", 1024),
		cql::NodeConfiguration().setAddress("d", 1024)
	});
	auto nodeA = nodeCollection.chooseOneNode();
	auto nodeB = nodeCollection.chooseOneNode();
	auto nodeC = nodeCollection.chooseOneNode();
	auto nodeD = nodeCollection.chooseOneNode();
	nodeCollection.reportFailure(nodeB);
	nodeCollection.reportFailure(nodeC);
	for (std::size_t i = 0; i < 3; ++i) {
		// retry b
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeB);
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeA);
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeD);
		// retry c
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeC);
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeA);
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeD);
	}
}

TEST(TestNodeCollectionImpl, chooseOneNodeWithAllIsFault) {
	cql::NodeCollectionImpl nodeCollection({
		cql::NodeConfiguration().setAddress("a", 1024),
		cql::NodeConfiguration().setAddress("b", 1024),
		cql::NodeConfiguration().setAddress("c", 1024),
	});
	auto nodeA = nodeCollection.chooseOneNode();
	auto nodeB = nodeCollection.chooseOneNode();
	auto nodeC = nodeCollection.chooseOneNode();
	nodeCollection.reportFailure(nodeA);
	nodeCollection.reportFailure(nodeB);
	nodeCollection.reportFailure(nodeC);
	for (std::size_t i = 0; i < 3; ++i) {
		// retry a, b, c
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeA);
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeB);
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeC);
	}
}

TEST(TestNodeCollectionImpl, chooseOneNodeWithFaultIsRecovered) {
	cql::NodeCollectionImpl nodeCollection({
		cql::NodeConfiguration().setAddress("a", 1024),
		cql::NodeConfiguration().setAddress("b", 1024),
		cql::NodeConfiguration().setAddress("c", 1024),
		cql::NodeConfiguration().setAddress("d", 1024),
	});
	auto nodeA = nodeCollection.chooseOneNode();
	auto nodeB = nodeCollection.chooseOneNode();
	auto nodeC = nodeCollection.chooseOneNode();
	auto nodeD = nodeCollection.chooseOneNode();
	nodeCollection.reportFailure(nodeA);
	nodeCollection.reportFailure(nodeB);
	nodeCollection.reportFailure(nodeC);
	nodeCollection.reportSuccess(nodeA);
	for (std::size_t i = 0; i < 3; ++i) {
		// retry b
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeB);
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeA);
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeD);
		// retry c
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeC);
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeA);
		ASSERT_TRUE(nodeCollection.chooseOneNode() == nodeD);
	}
}

