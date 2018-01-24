#include <Common/CqlNodeCollectionImpl.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlNodeCollectionImpl, construct) {
	cql::CqlNodeCollectionImpl nodeCollection({
		cql::CqlNodeConfiguration().setAddress("a", 1024),
		cql::CqlNodeConfiguration().setAddress("b", 1024),
		cql::CqlNodeConfiguration().setAddress("c", 1024)
	});
	ASSERT_EQ(nodeCollection.getNodesCount(), 3);
}

TEST(TestCqlNodeCollectionImpl, chooseOneNodeSimple) {
	cql::CqlNodeCollectionImpl nodeCollection({
		cql::CqlNodeConfiguration().setAddress("a", 1024),
		cql::CqlNodeConfiguration().setAddress("b", 1024),
		cql::CqlNodeConfiguration().setAddress("c", 1024)
	});
	for (std::size_t i = 0; i < 3; ++i) {
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "a");
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "b");
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "c");
	}
}

TEST(TestCqlNodeCollectionImpl, chooseOneNodeWithSomeIsFault) {
	cql::CqlNodeCollectionImpl nodeCollection({
		cql::CqlNodeConfiguration().setAddress("a", 1024),
		cql::CqlNodeConfiguration().setAddress("b", 1024),
		cql::CqlNodeConfiguration().setAddress("c", 1024),
		cql::CqlNodeConfiguration().setAddress("d", 1024)
	});
	ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "a");
	auto nodeB = nodeCollection.chooseOneNode();
	auto nodeC = nodeCollection.chooseOneNode();
	ASSERT_EQ(nodeB->getAddress().first, "b");
	ASSERT_EQ(nodeC->getAddress().first, "c");
	nodeCollection.reportFailure(nodeB);
	nodeCollection.reportFailure(nodeC);
	ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "d");
	for (std::size_t i = 0; i < 3; ++i) {
		// retry b
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "b");
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "a");
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "d");
		// retry c
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "c");
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "a");
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "d");
	}
}

TEST(TestCqlNodeCollectionImpl, chooseOneNodeWithAllIsFault) {
	cql::CqlNodeCollectionImpl nodeCollection({
		cql::CqlNodeConfiguration().setAddress("a", 1024),
		cql::CqlNodeConfiguration().setAddress("b", 1024),
		cql::CqlNodeConfiguration().setAddress("c", 1024),
	});
	auto nodeA = nodeCollection.chooseOneNode();
	auto nodeB = nodeCollection.chooseOneNode();
	auto nodeC = nodeCollection.chooseOneNode();
	ASSERT_EQ(nodeA->getAddress().first, "a");
	ASSERT_EQ(nodeB->getAddress().first, "b");
	ASSERT_EQ(nodeC->getAddress().first, "c");
	nodeCollection.reportFailure(nodeA);
	nodeCollection.reportFailure(nodeB);
	nodeCollection.reportFailure(nodeC);
	for (std::size_t i = 0; i < 3; ++i) {
		// retry a, b, c
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "a");
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "b");
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "c");
	}
}

TEST(TestCqlNodeCollectionImpl, chooseOneNodeWithFaultIsRecovered) {
	cql::CqlNodeCollectionImpl nodeCollection({
		cql::CqlNodeConfiguration().setAddress("a", 1024),
		cql::CqlNodeConfiguration().setAddress("b", 1024),
		cql::CqlNodeConfiguration().setAddress("c", 1024),
		cql::CqlNodeConfiguration().setAddress("d", 1024),
	});
	auto nodeA = nodeCollection.chooseOneNode();
	auto nodeB = nodeCollection.chooseOneNode();
	auto nodeC = nodeCollection.chooseOneNode();
	ASSERT_EQ(nodeA->getAddress().first, "a");
	ASSERT_EQ(nodeB->getAddress().first, "b");
	ASSERT_EQ(nodeC->getAddress().first, "c");
	nodeCollection.reportFailure(nodeA);
	nodeCollection.reportFailure(nodeB);
	nodeCollection.reportFailure(nodeC);
	nodeCollection.reportSuccess(nodeA);
	ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "d");
	for (std::size_t i = 0; i < 3; ++i) {
		// retry b
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "b");
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "a");
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "d");
		// retry c
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "c");
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "a");
		ASSERT_EQ(nodeCollection.chooseOneNode()->getAddress().first, "d");
	}
}

