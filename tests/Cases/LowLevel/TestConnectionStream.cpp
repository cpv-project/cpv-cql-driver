#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <LowLevel/ConnectionStream.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestConnectionStream, all) {
	{
		cql::ConnectionStream stream;
		ASSERT_FALSE(stream.isValid());
		ASSERT_THROWS(cql::LogicException, stream.getStreamId());
	}
	{
		auto freeStreamIds = seastar::make_lw_shared<std::vector<cql::ConnectionStream::IdType>>();
		{
			cql::ConnectionStream stream(1, freeStreamIds);
			ASSERT_TRUE(stream.isValid());
			ASSERT_EQ(stream.getStreamId(), 1);
		}
		ASSERT_EQ(freeStreamIds->size(), 1U);
		ASSERT_EQ(freeStreamIds->at(0), 1);
	}
	{
		auto freeStreamIds = seastar::make_lw_shared<std::vector<cql::ConnectionStream::IdType>>();
		// move construct
		cql::ConnectionStream original(2, freeStreamIds);
		cql::ConnectionStream testMoveConstruct(std::move(original));
		ASSERT_FALSE(original.isValid());
		ASSERT_TRUE(testMoveConstruct.isValid());
		ASSERT_EQ(testMoveConstruct.getStreamId(), 2);
		// move assign from invalid stream
		cql::ConnectionStream testMoveAssign;
		testMoveAssign = std::move(original);
		ASSERT_FALSE(testMoveAssign.isValid());
		// move assign from valid stream
		testMoveAssign = std::move(testMoveConstruct);
		ASSERT_FALSE(original.isValid());
		ASSERT_FALSE(testMoveConstruct.isValid());
		ASSERT_TRUE(testMoveAssign.isValid());
		ASSERT_EQ(testMoveAssign.getStreamId(), 2);
		// release stream by move assignment
		testMoveAssign = {};
		ASSERT_FALSE(testMoveAssign.isValid());
		ASSERT_EQ(freeStreamIds->size(), 1U);
		ASSERT_EQ(freeStreamIds->at(0), 2);
	}
}

