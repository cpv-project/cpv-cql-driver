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

TEST(TestCqlCommonDefinitions, columnTypeToString) {
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Custom), "Custom");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Ascii), "Ascii");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::BigInt), "BigInt");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Blob), "Blob");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Boolean), "Boolean");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Counter), "Counter");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Decimal), "Decimal");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Double), "Double");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Float), "Float");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Int), "Int");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Timestamp), "Timestamp");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Uuid), "Uuid");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::VarChar), "VarChar");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::VarInt), "VarInt");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::TimeUuid), "TimeUuid");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Inet), "Inet");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Date), "Date");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Time), "Time");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::SmallInt), "SmallInt");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::TinyInt), "TinyInt");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Duration), "Duration");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::List), "List");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Map), "Map");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Set), "Set");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Udt), "Udt");
	ASSERT_EQ(cql::joinString("", cql::CqlColumnType::Tuple), "Tuple");
}

