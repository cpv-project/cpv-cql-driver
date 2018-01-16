#include <LowLevel/RequestMessages/CqlQueryMessage.hpp>
#include <LowLevel/RequestMessages/CqlRequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlQueryMessage, encode) {
	auto message = cql::CqlRequestMessageFactory::makeRequestMessage<cql::CqlQueryMessage>();
	cql::CqlConnectionInfo info;
	seastar::sstring data;
	message->getQuery().set("use abc;", 8);
	message->getQueryParameters().setConsistency(cql::CqlConsistencyLevel::One);
	message->getHeader().encodeHeaderPre(info, data);
	message->encodeBody(info, data);
	message->getHeader().encodeHeaderPost(info, data);
	ASSERT_EQ(data, makeTestString(
		"\x04\x00\x00\x00\x07\x00\x00\x00\x12"
		"\x00\x00\x00\x08""use abc;"
		"\x00\x01\x00\x00\x00\x00"));
}

