#include <LowLevel/RequestMessages/ExecuteMessage.hpp>
#include <LowLevel/RequestMessages/RequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestExecuteMessage, encode) {
	cql::ConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::RequestMessageFactory::makeRequestMessage<cql::ExecuteMessage>();
		message->getPreparedQueryId().set("asd", 3);
		message->getQueryParameters().setCommand(
			cql::Command("")
				.setConsistency(cql::ConsistencyLevel::One));
		std::string data;
		message->getHeader().encodeHeaderPre(info, data);
		message->encodeBody(info, data);
		message->getHeader().encodeHeaderPost(info, data);
		ASSERT_EQ(data, makeTestString(
			"\x04\x00\x00\x00\x0a\x00\x00\x00\x08"
			"\x00\x03""asd"
			"\x00\x01\x00"));
	}
}

