#include <LowLevel/RequestMessages/RegisterMessage.hpp>
#include <LowLevel/RequestMessages/RequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestRegisterMessage, encode) {
	cql::ConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::RequestMessageFactory::makeRequestMessage<cql::RegisterMessage>();
		std::string data;
		message->getEvents().set({ cql::ProtocolString("TOPOLOGY_CHANGE") });
		message->getHeader().encodeHeaderPre(info, data);
		message->encodeBody(info, data);
		message->getHeader().encodeHeaderPost(info, data);
		ASSERT_EQ(data, makeTestString(
			"\x04\x00\x00\x00\x0b\x00\x00\x00\x13"
			"\x00\x01"
			"\x00\x0f""TOPOLOGY_CHANGE"));
	}
}

