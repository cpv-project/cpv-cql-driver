#include <LowLevel/ResponseMessages/CqlResultMessage.hpp>
#include <LowLevel/ResponseMessages/CqlResponseMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlResultMessage, decode) {
	cql::CqlConnectionInfo info;
	auto headerData = makeTestString("\x04\x00\x00\x00\x08\x00\x00\x00\x00");
	seastar::temporary_buffer<char> headerBuffer(headerData.data(), headerData.size());
	cql::CqlMessageHeader header;
	header.decodeHeader(info, std::move(headerBuffer));
	for (std::size_t i = 0; i < 3; ++i) {
		{
			// Void
			auto baseMessage = cql::CqlResponseMessageFactory::makeResponseMessage(cql::CqlMessageHeader(header));
			ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::CqlMessageType::Result);
			auto message = std::move(baseMessage).cast<cql::CqlResultMessage>();
			auto bodyData = makeTestString("\x00\x00\x00\x01");
			seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
			message->decodeBody(info, std::move(bodyBuffer));
			ASSERT_EQ(message->getKind(), cql::CqlResultKind::Void);
		}
		{
			// Rows
			auto baseMessage = cql::CqlResponseMessageFactory::makeResponseMessage(cql::CqlMessageHeader(header));
			ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::CqlMessageType::Result);
			auto message = std::move(baseMessage).cast<cql::CqlResultMessage>();
			auto bodyData = makeTestString(
				"\x00\x00\x00\x02"
				"\x00\x00\x00\x04""\x00\x00\x00\x03" // no metadata, 3 columns
				"\x00\x00\x00\x04" // 4 rows
				"\x00\x00\x00\x03""a_1""\x00\x00\x00\x03""a_2""\x00\x00\x00\x03""a_3"
				"\x00\x00\x00\x03""b_1""\x00\x00\x00\x03""b_2""\x00\x00\x00\x03""b_3"
				"\x00\x00\x00\x03""c_1""\x00\x00\x00\x03""c_2""\x00\x00\x00\x03""c_3"
				"\x00\x00\x00\x03""d_1""\x00\x00\x00\x03""d_2""\x00\x00\x00\x05""d_3__");
			seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
			message->decodeBody(info, std::move(bodyBuffer));
			ASSERT_EQ(message->getKind(), cql::CqlResultKind::Rows);
			ASSERT_EQ(message->getRowsMetadata().getFlags(), cql::CqlResultRowsMetadataFlags::NoMetadata);
			ASSERT_EQ(message->getRowsMetadata().getColumnsCount(), 3);
			ASSERT_EQ(message->getRowsCount().get(), 4);
			ASSERT_EQ(message->getRowsContents().size(), 12);
			ASSERT_EQ(message->getRowsContents().at(0).get(), "a_1");
			ASSERT_EQ(message->getRowsContents().at(1).get(), "a_2");
			ASSERT_EQ(message->getRowsContents().at(2).get(), "a_3");
			ASSERT_EQ(message->getRowsContents().at(3).get(), "b_1");
			ASSERT_EQ(message->getRowsContents().at(4).get(), "b_2");
			ASSERT_EQ(message->getRowsContents().at(5).get(), "b_3");
			ASSERT_EQ(message->getRowsContents().at(6).get(), "c_1");
			ASSERT_EQ(message->getRowsContents().at(7).get(), "c_2");
			ASSERT_EQ(message->getRowsContents().at(8).get(), "c_3");
			ASSERT_EQ(message->getRowsContents().at(9).get(), "d_1");
			ASSERT_EQ(message->getRowsContents().at(10).get(), "d_2");
			ASSERT_EQ(message->getRowsContents().at(11).get(), "d_3__");
		}
		{
			// SetKeySpace
			auto baseMessage = cql::CqlResponseMessageFactory::makeResponseMessage(cql::CqlMessageHeader(header));
			ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::CqlMessageType::Result);
			auto message = std::move(baseMessage).cast<cql::CqlResultMessage>();
			auto bodyData = makeTestString(
				"\x00\x00\x00\x03"
				"\x00\x01""k");
			seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
			message->decodeBody(info, std::move(bodyBuffer));
			ASSERT_EQ(message->getKind(), cql::CqlResultKind::SetKeySpace);
			ASSERT_EQ(message->getKeySpaceSet().get(), "k");
		}
		{
			// Prepared
			auto baseMessage = cql::CqlResponseMessageFactory::makeResponseMessage(cql::CqlMessageHeader(header));
			ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::CqlMessageType::Result);
			auto message = std::move(baseMessage).cast<cql::CqlResultMessage>();
			auto bodyData = makeTestString(
				"\x00\x00\x00\x04"
				"\x00\x03""abc"
				"\x00\x00\x00\x00" // metadata flags
				"\x00\x00\x00\x01" // 1 bind markers
				"\x00\x00\x00\x00" // 0 pk indexes
				"\x00\x01""k""\x00\x01""t""\x00\x01""c""\x00\x09"
				"\x00\x00\x00\x04""\x00\x00\x00\x03"); // no metadata, 3 columns
			seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
			message->decodeBody(info, std::move(bodyBuffer));
			ASSERT_EQ(message->getKind(), cql::CqlResultKind::Prepared);
			ASSERT_EQ(message->getPreparedQueryId().get(), "abc");
			ASSERT_EQ(message->getPreparedMetadata().getColumns().size(), 1);
			ASSERT_EQ(message->getPreparedMetadata().getColumns().at(0).getKeySpace(), "k");
			ASSERT_EQ(message->getPreparedMetadata().getColumns().at(0).getTable(), "t");
			ASSERT_EQ(message->getPreparedMetadata().getColumns().at(0).getName(), "c");
			ASSERT_EQ(message->getPreparedMetadata().getColumns().at(0).getType().get(), cql::CqlColumnType::Int);
			ASSERT_EQ(message->getPreparedRowsMetadata().getFlags(), cql::CqlResultRowsMetadataFlags::NoMetadata);
			ASSERT_EQ(message->getPreparedRowsMetadata().getColumnsCount(), 3);
		}
		{
			// SchemaChange
			auto baseMessage = cql::CqlResponseMessageFactory::makeResponseMessage(cql::CqlMessageHeader(header));
			ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::CqlMessageType::Result);
			auto message = std::move(baseMessage).cast<cql::CqlResultMessage>();
			auto bodyData = makeTestString(
				"\x00\x00\x00\x05"
				"\x00\x07""CREATED"
				"\x00\x08""KEYSPACE"
				"\x00\x03""abc");
			seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
			message->decodeBody(info, std::move(bodyBuffer));
			ASSERT_EQ(message->getKind(), cql::CqlResultKind::SchemaChange);
			ASSERT_EQ(message->getSchemaChangeType().get(), "CREATED");
			ASSERT_EQ(message->getSchemaChangeTarget().get(), "KEYSPACE");
			ASSERT_EQ(message->getSchemaChangeOptions(), makeTestString("\x00\x03""abc"));
		}
	}
}

