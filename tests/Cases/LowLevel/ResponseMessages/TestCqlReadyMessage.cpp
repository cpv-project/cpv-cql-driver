#include <LowLevel/ResponseMessages/CqlReadyMessage.hpp>
#include <LowLevel/ResponseMessages/CqlResponseMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlReadyMessage, decode) {
	cql::CqlConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto headerData = makeTestString("\x04\x00\x00\x00\x02\x00\x00\x00\x00");
		const char* headerDataPtr = headerData.data();
		const char* headerDataEnd = headerDataPtr + headerData.size();
		cql::CqlMessageHeader header;
		header.decodeHeader(info, headerDataPtr, headerDataEnd);

		auto baseMessage = cql::CqlResponseMessageFactory::makeResponseMessage(std::move(header));
		cql::CqlObject<cql::CqlReadyMessage> message(std::move(baseMessage));
 		auto bodyData = makeTestString("");
		const char* bodyDataPtr = bodyData.data();
		const char* bodyDataEnd = bodyDataPtr + bodyData.size();
		message->decodeBody(info, bodyDataPtr, bodyDataEnd);
	}
}

