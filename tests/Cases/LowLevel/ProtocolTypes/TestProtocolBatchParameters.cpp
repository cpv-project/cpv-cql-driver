#include <LowLevel/ProtocolTypes/ProtocolBatchParameters.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolBatchParameters, getset) {
	{
		cql::ProtocolBatchParameters value;
		value.setBatchCommand(
			cql::BatchCommand()
				.setConsistency(cql::ConsistencyLevel::One));
		ASSERT_EQ(value.getFlags(), cql::BatchParametersFlags::None);
		ASSERT_EQ(value.getBatchCommand().getConsistency(), cql::ConsistencyLevel::One);
	}
	{
		cql::ProtocolBatchParameters value;
		value.setBatchCommand(
			cql::BatchCommand()
				.setConsistency(cql::ConsistencyLevel::Two)
				.setSerialConsistency(cql::ConsistencyLevel::LocalSerial)
				.setDefaultTimestamp(
					std::chrono::system_clock::from_time_t(0) +
					std::chrono::microseconds(123)));
		ASSERT_EQ(value.getFlags(),
			cql::BatchParametersFlags::WithSerialConsistency |
			cql::BatchParametersFlags::WithDefaultTimestamp);
		ASSERT_EQ(value.getBatchCommand().getConsistency(), cql::ConsistencyLevel::Two);
		ASSERT_EQ(value.getBatchCommand().getSerialConsistency().first,
			cql::ConsistencyLevel::LocalSerial);
		ASSERT_TRUE(value.getBatchCommand().getSerialConsistency().second);
		ASSERT_EQ(value.getBatchCommand().getDefaultTimestamp().first,
			std::chrono::system_clock::from_time_t(0) + std::chrono::microseconds(123));
		ASSERT_TRUE(value.getBatchCommand().getDefaultTimestamp().second);
	}
}

TEST(TestProtocolBatchParameters, move) {
	cql::ProtocolBatchParameters a;
	a.setBatchCommand(cql::BatchCommand());
	ASSERT_TRUE(a.getBatchCommand().isValid());

	cql::ProtocolBatchParameters b = std::move(a);
	ASSERT_FALSE(a.getBatchCommand().isValid());
	ASSERT_TRUE(b.getBatchCommand().isValid());

	cql::BatchCommand batchCommand;
	b.setBatchCommandRef(batchCommand);
	ASSERT_EQ(&batchCommand, &b.getBatchCommand());
	ASSERT_TRUE(batchCommand.isValid());
	ASSERT_TRUE(b.getBatchCommand().isValid());

	a = std::move(b);
	ASSERT_EQ(&batchCommand, &a.getBatchCommand());
	ASSERT_TRUE(batchCommand.isValid());
	ASSERT_TRUE(a.getBatchCommand().isValid());
	ASSERT_FALSE(b.getBatchCommand().isValid());
}

TEST(TestProtocolBatchParameters, encode) {
	{
		cql::ProtocolBatchParameters value;
		value.setBatchCommand(
			cql::BatchCommand()
				.setConsistency(cql::ConsistencyLevel::One));
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x01"
			"\x00"));
	}
	{
		cql::ProtocolBatchParameters value;
		value.setBatchCommand(
			cql::BatchCommand()
				.setConsistency(cql::ConsistencyLevel::Two)
				.setSerialConsistency(cql::ConsistencyLevel::LocalSerial)
				.setDefaultTimestamp(
					std::chrono::system_clock::from_time_t(0) +
					std::chrono::microseconds(123)));
		std::string data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x02"
			"\x30"
			"\x00\x09"
			"\x00\x00\x00\x00\x00\x00\x00\x7b"));
	}
}

TEST(TestProtocolBatchParameters, decode) {
	cql::ProtocolBatchParameters value;
	{
		auto data = makeTestString(
			"\x00\x01"
			"\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getFlags(), cql::BatchParametersFlags::None);
		ASSERT_TRUE(value.getBatchCommand().isValid());
		ASSERT_EQ(value.getBatchCommand().getConsistency(), cql::ConsistencyLevel::One);
	}
	{
		auto data = makeTestString(
			"\x00\x02"
			"\x30"
			"\x00\x09"
			"\x00\x00\x00\x00\x00\x00\x00\x7b");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.getFlags(),
			cql::BatchParametersFlags::WithSerialConsistency |
			cql::BatchParametersFlags::WithDefaultTimestamp);
		ASSERT_TRUE(value.getBatchCommand().isValid());
		ASSERT_EQ(value.getBatchCommand().getConsistency(), cql::ConsistencyLevel::Two);
		ASSERT_EQ(value.getBatchCommand().getSerialConsistency().first,
			cql::ConsistencyLevel::LocalSerial);
		ASSERT_TRUE(value.getBatchCommand().getSerialConsistency().second);
		ASSERT_EQ(value.getBatchCommand().getDefaultTimestamp().first,
			std::chrono::system_clock::from_time_t(0) + std::chrono::microseconds(123));
		ASSERT_TRUE(value.getBatchCommand().getDefaultTimestamp().second);
	}
}

