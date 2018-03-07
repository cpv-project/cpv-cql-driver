#include <LowLevel/ResponseMessages/CqlErrorMessage.hpp>
#include <LowLevel/ResponseMessages/CqlResponseMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlErrorMessage, decode) {
	cql::CqlConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto headerData = makeTestString("\x04\x00\x00\x00\x00\x00\x00\x00\x00");
		seastar::temporary_buffer<char> headerBuffer(headerData.data(), headerData.size());
		cql::CqlMessageHeader header;
		header.decodeHeader(info, std::move(headerBuffer));

		auto baseMessage = cql::CqlResponseMessageFactory::makeResponseMessage(std::move(header));
		ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::CqlMessageType::Error);
		auto message = std::move(baseMessage).cast<cql::CqlErrorMessage>();
 		auto bodyData = makeTestString("\x00\x00\x00\x0a""\x00\x03""abc""123");
		seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
		message->decodeBody(info, std::move(bodyBuffer));
		ASSERT_EQ(message->getErrorCode(), cql::CqlErrorCode::ProtocolError);
		ASSERT_EQ(message->getErrorMessage().get(), "abc");
		ASSERT_EQ(message->getExtraContents(), "123");
	}
}

