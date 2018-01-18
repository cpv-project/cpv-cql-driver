#include <LowLevel/ResponseMessages/CqlErrorMessage.hpp>
#include <LowLevel/ResponseMessages/CqlResponseMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlErrorMessage, decode) {
	cql::CqlConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto headerData = makeTestString("\x04\x00\x00\x00\x00\x00\x00\x00\x00");
		cql::CqlMessageHeader header;
		header.decodeHeader(info, seastar::temporary_buffer<char>(headerData.data(), headerData.size()));

		auto baseMessage = cql::CqlResponseMessageFactory::makeResponseMessage(std::move(header));
		cql::CqlObject<cql::CqlErrorMessage> message(std::move(baseMessage));
 		seastar::temporary_buffer<char> data("\x00\x00\x00\x0a""\x00\x03""abc""123", 12);
		message->decodeBody(info, data);
		ASSERT_EQ(message->getErrorCode(), cql::CqlErrorCode::ProtocolError);
		ASSERT_EQ(message->getErrorMessage().get(), "abc");
		ASSERT_EQ(message->getExtraContents(), "123");
	}
}

