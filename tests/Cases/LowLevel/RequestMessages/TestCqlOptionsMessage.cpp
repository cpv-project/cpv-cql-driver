#include <LowLevel/RequestMessages/CqlOptionsMessage.hpp>
#include <LowLevel/RequestMessages/CqlRequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlOptionsMessage, encode) {
	cql::CqlConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::CqlRequestMessageFactory::makeRequestMessage<cql::CqlOptionsMessage>();
		seastar::sstring data;
		message->getHeader().encodeHeaderPre(info, data);
		message->encodeBody(info, data);
		message->getHeader().encodeHeaderPost(info, data);
		ASSERT_EQ(data, makeTestString("\x04\x00\x00\x00\x05\x00\x00\x00\x00"));
	}
}

