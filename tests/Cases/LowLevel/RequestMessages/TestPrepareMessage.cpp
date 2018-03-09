#include <LowLevel/RequestMessages/PrepareMessage.hpp>
#include <LowLevel/RequestMessages/RequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestPrepareMessage, encode) {
	cql::ConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::RequestMessageFactory::makeRequestMessage<cql::PrepareMessage>();
		seastar::sstring data;
		message->getQuery().set("use aaa;", 8);
		message->getHeader().encodeHeaderPre(info, data);
		message->encodeBody(info, data);
		message->getHeader().encodeHeaderPost(info, data);
		ASSERT_EQ(data, makeTestString(
			"\x04\x00\x00\x00\x09\x00\x00\x00\x0c"
			"\x00\x00\x00\x08""use aaa;"));
	}
}

