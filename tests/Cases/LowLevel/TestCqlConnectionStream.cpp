#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include <LowLevel/CqlConnectionStream.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlConnectionStream, all) {
	{
		cql::CqlConnectionStream stream;
		ASSERT_FALSE(stream.isValid());
		ASSERT_THROWS(cql::CqlLogicException, stream.getStreamId());
	}
	{
		auto freeStreamIds = seastar::make_lw_shared<std::vector<cql::CqlConnectionStream::IdType>>();
		{
			cql::CqlConnectionStream stream(1, freeStreamIds);
			ASSERT_TRUE(stream.isValid());
			ASSERT_EQ(stream.getStreamId(), 1);
		}
		ASSERT_EQ(freeStreamIds->size(), 1);
		ASSERT_EQ(freeStreamIds->at(0), 1);
	}
	{
		auto freeStreamIds = seastar::make_lw_shared<std::vector<cql::CqlConnectionStream::IdType>>();
		{
			// move construct
			cql::CqlConnectionStream original(2, freeStreamIds);
			cql::CqlConnectionStream testMoveConstruct(std::move(original));
			ASSERT_FALSE(original.isValid());
			ASSERT_TRUE(testMoveConstruct.isValid());
			ASSERT_EQ(testMoveConstruct.getStreamId(), 2);
			// move assign from invalid stream
			cql::CqlConnectionStream testMoveAssign;
			testMoveAssign = std::move(original);
			ASSERT_FALSE(testMoveAssign.isValid());
			// move assign from valid stream
			testMoveAssign = std::move(testMoveConstruct);
			ASSERT_FALSE(original.isValid());
			ASSERT_FALSE(testMoveConstruct.isValid());
			ASSERT_TRUE(testMoveAssign.isValid());
			ASSERT_EQ(testMoveAssign.getStreamId(), 2);
		}
		ASSERT_EQ(freeStreamIds->size(), 1);
		ASSERT_EQ(freeStreamIds->at(0), 2);
	}
}

