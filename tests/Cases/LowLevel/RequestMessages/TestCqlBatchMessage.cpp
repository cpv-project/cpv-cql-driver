#include <CqlDriver/Common/ColumnTypes/CqlInt.hpp>
#include <CqlDriver/Common/ColumnTypes/CqlText.hpp>
#include <LowLevel/RequestMessages/CqlBatchMessage.hpp>
#include <LowLevel/RequestMessages/CqlRequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlBatchMessage, encode) {
	cql::CqlConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::CqlRequestMessageFactory::makeRequestMessage<cql::CqlBatchMessage>();
		seastar::sstring data;
		message->setType(cql::CqlBatchType::UnLogged);
		message->getBatchParameters().setBatchCommand(
			cql::CqlBatchCommand()
				.setConsistencyLevel(cql::CqlConsistencyLevel::One)
				.setSerialConsistencyLevel(cql::CqlConsistencyLevel::LocalSerial)
				.addQuery("abc")
				.addQuery("query aaa")
				.addParameters(cql::CqlText("1")));
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
		auto message = cql::CqlRequestMessageFactory::makeRequestMessage<cql::CqlBatchMessage>();
		seastar::sstring data;
		message->setType(cql::CqlBatchType::Logged);
		message->getBatchParameters().setBatchCommand(
			cql::CqlBatchCommand()
				.setConsistencyLevel(cql::CqlConsistencyLevel::Two)
				.addQuery("abc")
					.openParameterSet()
						.addParameters(cql::CqlInt(1))
					.openParameterSet()
						.addParameters(cql::CqlInt(2))
					.openParameterSet()
						.addParameters(cql::CqlInt(3))
				.addQuery("query aaa")
					.openParameterSet()
						.addParameters(cql::CqlInt(1))
					.openParameterSet()
						.addParameters(cql::CqlInt(2)));
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

