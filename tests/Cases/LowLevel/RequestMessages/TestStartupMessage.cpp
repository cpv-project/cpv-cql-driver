#include <LowLevel/RequestMessages/StartupMessage.hpp>
#include <LowLevel/RequestMessages/RequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestStartupMessage, encode) {
	cql::ConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		{
			auto message = cql::RequestMessageFactory::makeRequestMessage<cql::StartupMessage>();
			std::string data;
			message->getHeader().encodeHeaderPre(info, data);
			message->encodeBody(info, data);
			message->getHeader().encodeHeaderPost(info, data);
			ASSERT_EQ(data, makeTestString(
				"\x04\x00\x00\x00\x01\x00\x00\x00\x16"
				"\x00\x01"
				"\x00\x0b""CQL_VERSION""\x00\x05""3.0.0"));
		}
		{
			auto message = cql::RequestMessageFactory::makeRequestMessage<cql::StartupMessage>();
			message->setCompression("snappy");
			std::string data;
			message->getHeader().encodeHeaderPre(info, data);
			message->encodeBody(info, data);
			message->getHeader().encodeHeaderPost(info, data);
			auto patternA = makeTestString(
				"\x04\x00\x00\x00\x01\x00\x00\x00\x2b"
				"\x00\x02"
				"\x00\x0b""CQL_VERSION""\x00\x05""3.0.0"
				"\x00\x0b""COMPRESSION""\x00\x06""snappy");
			auto patternB = makeTestString(
				"\x04\x00\x00\x00\x01\x00\x00\x00\x2b"
				"\x00\x02"
				"\x00\x0b""COMPRESSION""\x00\x06""snappy"
				"\x00\x0b""CQL_VERSION""\x00\x05""3.0.0");
			ASSERT_TRUE(data == patternA || data == patternB); 
		}
	}
}

