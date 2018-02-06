#include <CqlDriver/Common/Utility/MathUtils.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(MathUtils, powInteger) {
	ASSERT_EQ(cql::powInteger(2, 0), 1);
	ASSERT_EQ(cql::powInteger(2, 1), 2);
	ASSERT_EQ(cql::powInteger(2, 2), 4);
	ASSERT_EQ(cql::powInteger(2, 3), 8);
	ASSERT_EQ(cql::powInteger(2, 4), 16);
	ASSERT_EQ(cql::powInteger(2, 5), 32);
}

