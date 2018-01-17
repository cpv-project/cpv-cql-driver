#include <LowLevel/RequestMessages/CqlExecuteMessage.hpp>
#include <LowLevel/RequestMessages/CqlRequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlExecuteMessage, encode) {
	cql::CqlConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::CqlRequestMessageFactory::makeRequestMessage<cql::CqlExecuteMessage>();
		seastar::sstring data;
		message->getPreparedQueryId().set("asd", 3);
		message->getResultMetadataId().set("a", 1);
		message->getQueryParameters().setConsistency(cql::CqlConsistencyLevel::One);
		message->getHeader().encodeHeaderPre(info, data);
		message->encodeBody(info, data);
		message->getHeader().encodeHeaderPost(info, data);
		ASSERT_EQ(data, makeTestString(
			"\x04\x00\x00\x00\x0a\x00\x00\x00\x0b"
			"\x00\x03""asd"
			"\x00\x01""a"
			"\x00\x01\x00"));
	}
}

