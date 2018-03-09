#include <LowLevel/ResponseMessages/AuthChallengeMessage.hpp>
#include <LowLevel/ResponseMessages/ResponseMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestAuthChallengeMessage, decode) {
	cql::ConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto headerData = makeTestString("\x04\x00\x00\x00\x0e\x00\x00\x00\x00");
		seastar::temporary_buffer<char> headerBuffer(headerData.data(), headerData.size());
		cql::MessageHeader header;
		header.decodeHeader(info, std::move(headerBuffer));

		auto baseMessage = cql::ResponseMessageFactory::makeResponseMessage(std::move(header));
		ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::MessageType::AuthChallenge);
		auto message = std::move(baseMessage).cast<cql::AuthChallengeMessage>();
 		auto bodyData = makeTestString("\x00\x00\x00\x03""abc");
		seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
		message->decodeBody(info, std::move(bodyBuffer));
		ASSERT_EQ(message->getToken().get(), "abc");
	}
}

