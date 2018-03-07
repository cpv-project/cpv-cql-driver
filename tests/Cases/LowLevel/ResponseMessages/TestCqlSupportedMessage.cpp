#include <LowLevel/ResponseMessages/CqlSupportedMessage.hpp>
#include <LowLevel/ResponseMessages/CqlResponseMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlSupportedMessage, decode) {
	cql::CqlConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto headerData = makeTestString("\x04\x00\x00\x00\x06\x00\x00\x00\x00");
		seastar::temporary_buffer<char> headerBuffer(headerData.data(), headerData.size());
		cql::CqlMessageHeader header;
		header.decodeHeader(info, std::move(headerBuffer));

		auto baseMessage = cql::CqlResponseMessageFactory::makeResponseMessage(std::move(header));
		ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::CqlMessageType::Supported);
		auto message = std::move(baseMessage).cast<cql::CqlSupportedMessage>();
 		auto bodyData = makeTestString(
			"\x00\x02"
			"\x00\x05""apple""\x00\x02""\x00\x04""dogA""\x00\x04""dogB"
			"\x00\x06""orange""\x00\x01""\x00\x03""cat");
		seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
		message->decodeBody(info, std::move(bodyBuffer));
		auto& options = message->getOptions();
		ASSERT_EQ(options.get().size(), 2);
		ASSERT_EQ(options.get().at(cql::CqlProtocolString("apple")).get().size(), 2);
		ASSERT_EQ(options.get().at(cql::CqlProtocolString("apple")).get().at(0).get(), "dogA");
		ASSERT_EQ(options.get().at(cql::CqlProtocolString("apple")).get().at(1).get(), "dogB");
		ASSERT_EQ(options.get().at(cql::CqlProtocolString("orange")).get().size(), 1);
		ASSERT_EQ(options.get().at(cql::CqlProtocolString("orange")).get().at(0).get(), "cat");
	}
}

