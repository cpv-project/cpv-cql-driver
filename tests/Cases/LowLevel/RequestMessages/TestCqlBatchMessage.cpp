#include <LowLevel/RequestMessages/CqlBatchMessage.hpp>
#include <LowLevel/RequestMessages/CqlRequestMessageFactory.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlBatchMessage, encode) {
	cql::CqlConnectionInfo info;
	for (std::size_t i = 0; i < 3; ++i) {
		auto message = cql::CqlRequestMessageFactory::makeRequestMessage<cql::CqlBatchMessage>();
		seastar::sstring data;
		message->setType(cql::CqlBatchType::UnLogged);
		cql::CqlProtocolBatchQuery queryA;
		queryA.setQuery("abc");
		cql::CqlProtocolBatchQuery queryB;
		queryB.setPreparedQueryId("aaa");
		queryB.setValues({ cql::CqlProtocolValue("1") });
		message->getQueries().get() = { queryA, queryB };
		message->getBatchParameters().setConsistency(cql::CqlConsistencyLevel::One);
		message->getBatchParameters().setSerialConsistency(cql::CqlConsistencyLevel::LocalSerial);
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
}

