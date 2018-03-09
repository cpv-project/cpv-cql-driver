#include <CQLDriver/Common/ColumnTypes/Int.hpp>
#include <CQLDriver/Common/ColumnTypes/Text.hpp>
#include <LowLevel/RequestMessages/BatchMessage.hpp>
#include <LowLevel/RequestMessages/RequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestBatchMessage, encode) {
	cql::ConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::RequestMessageFactory::makeRequestMessage<cql::BatchMessage>();
		seastar::sstring data;
		message->setType(cql::BatchType::UnLogged);
		message->getBatchParameters().setBatchCommand(
			cql::BatchCommand()
				.setConsistency(cql::ConsistencyLevel::One)
				.setSerialConsistency(cql::ConsistencyLevel::LocalSerial)
				.addQuery("abc")
				.addQuery("query aaa")
				.addParameters(cql::Text("1")));
		message->getPreparedQueryId(1) = "aaa";
		message->getHeader().encodeHeaderPre(info, data);
		message->encodeBody(info, data);
		message->getHeader().encodeHeaderPost(info, data);
		ASSERT_EQ(data, makeTestString(
			"\x04\x00\x00\x00\x0d\x00\x00\x00\x1f"
			"\x01"
			"\x00\x02"
			"\x00\x00\x00\x00\x03""abc""\x00\x00"
			"\x01\x00\x03""aaa""\x00\x01""\x00\x00\x00\x01""1"
			"\x00\x01\x10\x00\x09"));
	}
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::RequestMessageFactory::makeRequestMessage<cql::BatchMessage>();
		seastar::sstring data;
		message->setType(cql::BatchType::Logged);
		message->getBatchParameters().setBatchCommand(
			cql::BatchCommand()
				.setConsistency(cql::ConsistencyLevel::Two)
				.addQuery("abc")
					.openParameterSet()
						.addParameters(cql::Int(1))
					.openParameterSet()
						.addParameters(cql::Int(2))
					.openParameterSet()
						.addParameters(cql::Int(3))
				.addQuery("query aaa")
					.openParameterSet()
						.addParameters(cql::Int(1))
					.openParameterSet()
						.addParameters(cql::Int(2)));
		message->getPreparedQueryId(1) = "aaa";
		message->getHeader().encodeHeaderPre(info, data);
		message->encodeBody(info, data);
		message->getHeader().encodeHeaderPost(info, data);
		ASSERT_EQ(data, makeTestString(
			"\x04\x00\x00\x00\x0d\x00\x00\x00\x5c"
			"\x00"
			"\x00\x05"
			"\x00\x00\x00\x00\x03""abc""\x00\x01""\x00\x00\x00\x04\x00\x00\x00\x01"
			"\x00\x00\x00\x00\x03""abc""\x00\x01""\x00\x00\x00\x04\x00\x00\x00\x02"
			"\x00\x00\x00\x00\x03""abc""\x00\x01""\x00\x00\x00\x04\x00\x00\x00\x03"
			"\x01\x00\x03""aaa""\x00\x01""\x00\x00\x00\x04\x00\x00\x00\x01"
			"\x01\x00\x03""aaa""\x00\x01""\x00\x00\x00\x04\x00\x00\x00\x02"
			"\x00\x02\x00"));
	}
}

