#include <LowLevel/RequestMessages/OptionsMessage.hpp>
#include <LowLevel/RequestMessages/RequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestOptionsMessage, encode) {
	cql::ConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::RequestMessageFactory::makeRequestMessage<cql::OptionsMessage>();
		std::string data;
		message->getHeader().encodeHeaderPre(info, data);
		message->encodeBody(info, data);
		message->getHeader().encodeHeaderPost(info, data);
		ASSERT_EQ(data, makeTestString("\x04\x00\x00\x00\x05\x00\x00\x00\x00"));
	}
}

