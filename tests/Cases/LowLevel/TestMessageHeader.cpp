#include <LowLevel/MessageHeader.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestMessageHeader, getset) {
	cql::MessageHeader header;
	header.setFlags(cql::MessageHeaderFlags::Compression);
	header.setStreamId(123);
	header.setOpCode(cql::MessageType::Startup, cql::MessageDirection::Request);
	ASSERT_EQ(header.getFlags(), cql::MessageHeaderFlags::Compression);
	ASSERT_EQ(header.getStreamId(), 123);
	ASSERT_EQ(header.getOpCode(), cql::MessageType::Startup);
	ASSERT_EQ(header.getDirection(), cql::MessageDirection::Request);
	ASSERT_EQ(header.getBodyLength(), 0);
}

TEST(TestMessageHeader, encode) {
	cql::MessageHeader header;
	cql::ConnectionInfo info;
	header.setFlags(cql::MessageHeaderFlags::Compression);
	header.setStreamId(123);
	header.setOpCode(cql::MessageType::Startup, cql::MessageDirection::Request);
	std::string data;
	header.encodeHeaderPre(info, data);
	data.append("asdfg", 5);
	header.encodeHeaderPost(info, data);
	ASSERT_EQ(data, std::string("\x04\x01\x00\x7b\x01\x00\x00\x00\x05""asdfg", 9+5));
}

TEST(TestMessageHeader, decode) {
	cql::MessageHeader header;
	cql::ConnectionInfo info;
	auto data = makeTestString("\x84\x01\x00\x7b\x02\x00\x00\x00\x08");
	seastar::temporary_buffer<char> buffer(data.data(), data.size());
	header.decodeHeader(info, std::move(buffer));
	ASSERT_EQ(header.getFlags(), cql::MessageHeaderFlags::Compression);
	ASSERT_EQ(header.getStreamId(), 123);
	ASSERT_EQ(header.getOpCode(), cql::MessageType::Ready);
	ASSERT_EQ(header.getDirection(), cql::MessageDirection::Response);
	ASSERT_EQ(header.getBodyLength(), 8);
}

