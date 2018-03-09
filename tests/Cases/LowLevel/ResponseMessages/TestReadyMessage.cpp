#include <LowLevel/ResponseMessages/ReadyMessage.hpp>
#include <LowLevel/ResponseMessages/ResponseMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestReadyMessage, decode) {
	cql::ConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto headerData = makeTestString("\x04\x00\x00\x00\x02\x00\x00\x00\x00");
		seastar::temporary_buffer<char> headerBuffer(headerData.data(), headerData.size());
		cql::MessageHeader header;
		header.decodeHeader(info, std::move(headerBuffer));

		auto baseMessage = cql::ResponseMessageFactory::makeResponseMessage(std::move(header));
		ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::MessageType::Ready);
		auto message = std::move(baseMessage).cast<cql::ReadyMessage>();
 		auto bodyData = makeTestString("");
		seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
		message->decodeBody(info, std::move(bodyBuffer));
	}
}

