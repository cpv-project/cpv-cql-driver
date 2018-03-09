#include <LowLevel/ResponseMessages/ErrorMessage.hpp>
#include <LowLevel/ResponseMessages/ResponseMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestErrorMessage, decode) {
	cql::ConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto headerData = makeTestString("\x04\x00\x00\x00\x00\x00\x00\x00\x00");
		seastar::temporary_buffer<char> headerBuffer(headerData.data(), headerData.size());
		cql::MessageHeader header;
		header.decodeHeader(info, std::move(headerBuffer));

		auto baseMessage = cql::ResponseMessageFactory::makeResponseMessage(std::move(header));
		ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::MessageType::Error);
		auto message = std::move(baseMessage).cast<cql::ErrorMessage>();
 		auto bodyData = makeTestString("\x00\x00\x00\x0a""\x00\x03""abc""123");
		seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
		message->decodeBody(info, std::move(bodyBuffer));
		ASSERT_EQ(message->getErrorCode(), cql::ErrorCode::ProtocolError);
		ASSERT_EQ(message->getErrorMessage().get(), "abc");
		ASSERT_EQ(message->getExtraContents(), "123");
	}
}

