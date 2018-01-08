#include <CqlDriver/Common/Utility/StringUtils.hpp>
#include <LowLevel/CqlDefinitions.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlDefinitions, messageTypeToString) {
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::Error), "Error");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::StartUp), "StartUp");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::Ready), "Ready");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::Authenticate), "Authenticate");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::Options), "Options");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::Supported), "Supported");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::Query), "Query");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::Result), "Result");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::Prepare), "Prepare");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::Execute), "Execute");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::Register), "Register");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::Event), "Event");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::Batch), "Batch");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::AuthChallenge), "AuthChallenge");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::AuthResponse), "AuthResponse");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType::AuthSuccess), "AuthSuccess");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageType(-1)), "Unknow");
}

TEST(TestCqlDefinitions, messageHeaderFlagsToString) {
	ASSERT_EQ(cql::joinString("", cql::CqlMessageHeaderFlags::None), "");
	ASSERT_EQ(
		cql::joinString("", cql::CqlMessageHeaderFlags::Compression | cql::CqlMessageHeaderFlags::Tracing),
		"Compression|Tracing");
	ASSERT_EQ(
		cql::joinString("", cql::CqlMessageHeaderFlags::CustomPayload | cql::CqlMessageHeaderFlags::Warning),
		"CustomPayload|Warning");
	ASSERT_EQ(cql::joinString("", cql::CqlMessageHeaderFlags::UseBeta), "UseBeta");
}

