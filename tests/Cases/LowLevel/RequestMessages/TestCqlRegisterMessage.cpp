#include <LowLevel/RequestMessages/CqlRegisterMessage.hpp>
#include <LowLevel/RequestMessages/CqlRequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlRegisterMessage, encode) {
	cql::CqlConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::CqlRequestMessageFactory::makeRequestMessage<cql::CqlRegisterMessage>();
		seastar::sstring data;
		message->getEvents().set({ cql::CqlProtocolString("TOPOLOGY_CHANGE") });
		message->getHeader().encodeHeaderPre(info, data);
		message->encodeBody(info, data);
		message->getHeader().encodeHeaderPost(info, data);
		ASSERT_EQ(data, makeTestString(
			"\x04\x00\x00\x00\x0b\x00\x00\x00\x13"
			"\x00\x01"
			"\x00\x0f""TOPOLOGY_CHANGE"));
	}
}

