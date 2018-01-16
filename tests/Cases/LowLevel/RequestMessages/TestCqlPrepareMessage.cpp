#include <LowLevel/RequestMessages/CqlPrepareMessage.hpp>
#include <LowLevel/RequestMessages/CqlRequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlPrepareMessage, encode) {
	cql::CqlConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::CqlRequestMessageFactory::makeRequestMessage<cql::CqlPrepareMessage>();
		seastar::sstring data;
		message->getQuery().set("use aaa;", 8);
		message->getPrepareParameters().setKeySpace("asdfg");
		message->getHeader().encodeHeaderPre(info, data);
		message->encodeBody(info, data);
		message->getHeader().encodeHeaderPost(info, data);
		ASSERT_EQ(data, makeTestString(
			"\x04\x00\x00\x00\x09\x00\x00\x00\x17"
			"\x00\x00\x00\x08""use aaa;"
			"\x00\x00\x00\x01"
			"\x00\x05""asdfg"));
	}
}

