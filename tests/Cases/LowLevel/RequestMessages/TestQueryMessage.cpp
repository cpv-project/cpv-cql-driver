#include <LowLevel/RequestMessages/QueryMessage.hpp>
#include <LowLevel/RequestMessages/RequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestQueryMessage, encode) {
	cql::ConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::RequestMessageFactory::makeRequestMessage<cql::QueryMessage>();
		message->getQueryParameters().setCommand(
			cql::Command("use abc;")
				.setConsistency(cql::ConsistencyLevel::One));
		seastar::sstring data;
		message->getHeader().encodeHeaderPre(info, data);
		message->encodeBody(info, data);
		message->getHeader().encodeHeaderPost(info, data);
		ASSERT_EQ(data, makeTestString(
			"\x04\x00\x00\x00\x07\x00\x00\x00\x0f"
			"\x00\x00\x00\x08""use abc;"
			"\x00\x01\x00"));
	}
}

