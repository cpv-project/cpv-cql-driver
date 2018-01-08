#include <CqlDriver/Common/Utility/StringUtils.hpp>
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlCommonDefinitions, consistecyLevelToString) {
	ASSERT_EQ(cql::joinString("", cql::CqlConsistencyLevel::Any), "Any");
	ASSERT_EQ(cql::joinString("", cql::CqlConsistencyLevel::One), "One");
	ASSERT_EQ(cql::joinString("", cql::CqlConsistencyLevel::Two), "Two");
	ASSERT_EQ(cql::joinString("", cql::CqlConsistencyLevel::Three), "Three");
	ASSERT_EQ(cql::joinString("", cql::CqlConsistencyLevel::Quorum), "Quorum");
	ASSERT_EQ(cql::joinString("", cql::CqlConsistencyLevel::All), "All");
	ASSERT_EQ(cql::joinString("", cql::CqlConsistencyLevel::LocalQuorum), "LocalQuorum");
	ASSERT_EQ(cql::joinString("", cql::CqlConsistencyLevel::EachQuorum), "EachQuorum");
	ASSERT_EQ(cql::joinString("", cql::CqlConsistencyLevel::Serial), "Serial");
	ASSERT_EQ(cql::joinString("", cql::CqlConsistencyLevel::LocalSerial), "LocalSerial");
	ASSERT_EQ(cql::joinString("", cql::CqlConsistencyLevel::LocalOne), "LocalOne");
}

