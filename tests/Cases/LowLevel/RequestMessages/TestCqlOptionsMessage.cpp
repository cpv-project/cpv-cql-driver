#include <LowLevel/RequestMessages/CqlOptionsMessage.hpp>
#include <LowLevel/RequestMessages/CqlRequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlOptionsMessage, encode) {
	auto message = cql::CqlRequestMessageFactory::makeRequestMessage<cql::CqlOptionsMessage>();
	cql::CqlConnectionInfo info;
	seastar::sstring data;
	message->getHeader().encodeHeaderPre(info, data);
	message->encodeBody(info, data);
	message->getHeader().encodeHeaderPost(info, data);
	ASSERT_EQ(data, seastar::sstring("\x04\x00\x00\x00\x05\x00\x00\x00\x00", 9));
}

