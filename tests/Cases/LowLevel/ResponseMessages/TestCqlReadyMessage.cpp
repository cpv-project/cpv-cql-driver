#include <LowLevel/ResponseMessages/CqlReadyMessage.hpp>
#include <LowLevel/ResponseMessages/CqlResponseMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlReadyMessage, decode) {
	cql::CqlConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto headerData = makeTestString("\x04\x00\x00\x00\x02\x00\x00\x00\x00");
		seastar::temporary_buffer<char> headerBuffer(headerData.data(), headerData.size());
		cql::CqlMessageHeader header;
		header.decodeHeader(info, std::move(headerBuffer));

		auto baseMessage = cql::CqlResponseMessageFactory::makeResponseMessage(std::move(header));
		ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::CqlMessageType::Ready);
		cql::CqlObject<cql::CqlReadyMessage> message(std::move(baseMessage));
 		auto bodyData = makeTestString("");
		seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
		message->decodeBody(info, std::move(bodyBuffer));
	}
}

