#include <LowLevel/ResponseMessages/ResultMessage.hpp>
#include <LowLevel/ResponseMessages/ResponseMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestResultMessage, decode) {
	cql::ConnectionInfo info;
	auto headerData = makeTestString("\x04\x00\x00\x00\x08\x00\x00\x00\x00");
	seastar::temporary_buffer<char> headerBuffer(headerData.data(), headerData.size());
	cql::MessageHeader header;
	header.decodeHeader(info, std::move(headerBuffer));
	for (std::size_t i = 0; i < 3; ++i) {
		{
			// Void
			auto baseMessage = cql::ResponseMessageFactory::makeResponseMessage(cql::MessageHeader(header));
			ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::MessageType::Result);
			auto message = std::move(baseMessage).cast<cql::ResultMessage>();
			auto bodyData = makeTestString("\x00\x00\x00\x01");
			seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
			message->decodeBody(info, std::move(bodyBuffer));
			ASSERT_EQ(message->getKind(), cql::ResultKind::Void);
		}
		{
			// Rows
			auto baseMessage = cql::ResponseMessageFactory::makeResponseMessage(cql::MessageHeader(header));
			ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::MessageType::Result);
			auto message = std::move(baseMessage).cast<cql::ResultMessage>();
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
			ASSERT_EQ(message->getKind(), cql::ResultKind::Rows);
			ASSERT_EQ(message->getRowsMetadata().getFlags(), cql::ResultRowsMetadataFlags::NoMetadata);
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
			auto baseMessage = cql::ResponseMessageFactory::makeResponseMessage(cql::MessageHeader(header));
			ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::MessageType::Result);
			auto message = std::move(baseMessage).cast<cql::ResultMessage>();
			auto bodyData = makeTestString(
				"\x00\x00\x00\x03"
				"\x00\x01""k");
			seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
			message->decodeBody(info, std::move(bodyBuffer));
			ASSERT_EQ(message->getKind(), cql::ResultKind::SetKeySpace);
			ASSERT_EQ(message->getKeySpaceSet().get(), "k");
		}
		{
			// Prepared
			auto baseMessage = cql::ResponseMessageFactory::makeResponseMessage(cql::MessageHeader(header));
			ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::MessageType::Result);
			auto message = std::move(baseMessage).cast<cql::ResultMessage>();
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
			ASSERT_EQ(message->getKind(), cql::ResultKind::Prepared);
			ASSERT_EQ(message->getPreparedQueryId().get(), "abc");
			ASSERT_EQ(message->getPreparedMetadata().getColumns().size(), 1);
			ASSERT_EQ(message->getPreparedMetadata().getColumns().at(0).getKeySpace(), "k");
			ASSERT_EQ(message->getPreparedMetadata().getColumns().at(0).getTable(), "t");
			ASSERT_EQ(message->getPreparedMetadata().getColumns().at(0).getName(), "c");
			ASSERT_EQ(message->getPreparedMetadata().getColumns().at(0).getType().get(), cql::ColumnType::Int);
			ASSERT_EQ(message->getPreparedRowsMetadata().getFlags(), cql::ResultRowsMetadataFlags::NoMetadata);
			ASSERT_EQ(message->getPreparedRowsMetadata().getColumnsCount(), 3);
		}
		{
			// SchemaChange
			auto baseMessage = cql::ResponseMessageFactory::makeResponseMessage(cql::MessageHeader(header));
			ASSERT_EQ(baseMessage->getHeader().getOpCode(), cql::MessageType::Result);
			auto message = std::move(baseMessage).cast<cql::ResultMessage>();
			auto bodyData = makeTestString(
				"\x00\x00\x00\x05"
				"\x00\x07""CREATED"
				"\x00\x08""KEYSPACE"
				"\x00\x03""abc");
			seastar::temporary_buffer<char> bodyBuffer(bodyData.data(), bodyData.size());
			message->decodeBody(info, std::move(bodyBuffer));
			ASSERT_EQ(message->getKind(), cql::ResultKind::SchemaChange);
			ASSERT_EQ(message->getSchemaChangeType().get(), "CREATED");
			ASSERT_EQ(message->getSchemaChangeTarget().get(), "KEYSPACE");
			ASSERT_EQ(message->getSchemaChangeOptions(), makeTestString("\x00\x03""abc"));
		}
	}
}

