#include <LowLevel/CqlMessageHeader.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlMessageHeader, getset) {
	cql::CqlMessageHeader header;
	header.setFlags(cql::CqlMessageHeaderFlags::Compression);
	header.setStreamId(123);
	header.setOpCode(cql::CqlMessageType::Startup, cql::CqlMessageDirection::Request);
	ASSERT_EQ(header.getFlags(), cql::CqlMessageHeaderFlags::Compression);
	ASSERT_EQ(header.getStreamId(), 123);
	ASSERT_EQ(header.getOpCode(), cql::CqlMessageType::Startup);
	ASSERT_EQ(header.getDirection(), cql::CqlMessageDirection::Request);
	ASSERT_EQ(header.getBodyLength(), 0);
}

TEST(TestCqlMessageHeader, encode) {
	cql::CqlMessageHeader header;
	cql::CqlConnectionInfo info;
	header.setFlags(cql::CqlMessageHeaderFlags::Compression);
	header.setStreamId(123);
	header.setOpCode(cql::CqlMessageType::Startup, cql::CqlMessageDirection::Request);
	seastar::sstring data;
	header.encodeHeaderPre(info, data);
	data.append("asdfg", 5);
	header.encodeHeaderPost(info, data);
	ASSERT_EQ(data, seastar::sstring("\x04\x01\x00\x7b\x01\x00\x00\x00\x05""asdfg", 9+5));
}

TEST(TestCqlMessageHeader, decode) {
	cql::CqlMessageHeader header;
	cql::CqlConnectionInfo info;
	auto data = makeTestString("\x84\x01\x00\x7b\x02\x00\x00\x00\x08");
	seastar::temporary_buffer<char> buffer(data.data(), data.size());
	header.decodeHeader(info, std::move(buffer));
	ASSERT_EQ(header.getFlags(), cql::CqlMessageHeaderFlags::Compression);
	ASSERT_EQ(header.getStreamId(), 123);
	ASSERT_EQ(header.getOpCode(), cql::CqlMessageType::Ready);
	ASSERT_EQ(header.getDirection(), cql::CqlMessageDirection::Response);
	ASSERT_EQ(header.getBodyLength(), 8);
}

