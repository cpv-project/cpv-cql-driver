#include <LowLevel/RequestMessages/CqlAuthResponseMessage.hpp>
#include <LowLevel/RequestMessages/CqlRequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlAuthResponseMessage, encode) {
	cql::CqlConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::CqlRequestMessageFactory::makeRequestMessage<cql::CqlAuthResponseMessage>();
		message->getToken().set("abc", 3);
		seastar::sstring data;
		message->getHeader().encodeHeaderPre(info, data);
		message->encodeBody(info, data);
		message->getHeader().encodeHeaderPost(info, data);
		ASSERT_EQ(data, makeTestString("\x04\x00\x00\x00\x0f\x00\x00\x00\x07""\x00\x00\x00\x03""abc"));
	}
}

