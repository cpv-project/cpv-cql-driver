#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <LowLevel/ProtocolTypes/ProtocolColumnOption.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestProtocolColumnOption, getset) {
	cql::ProtocolColumnOption value(cql::ColumnType::Ascii);
	ASSERT_EQ(value.get(), cql::ColumnType::Ascii);
	value.set(cql::ColumnType::Int);
	ASSERT_EQ(value.get(), cql::ColumnType::Int);

	value = cql::ProtocolColumnOption();
	ASSERT_EQ(value.get(), cql::ColumnType::Custom);
}

TEST(TestProtocolColumnOption, getsetCustomPayload) {
	cql::ProtocolColumnOption value(
		cql::ProtocolColumnOptionCustomPayload(
			cql::ProtocolString("abc")));
	ASSERT_EQ(value.get(), cql::ColumnType::Custom);
	auto payload = value.getCustomPayload();
	ASSERT_EQ(payload.get().get(), "abc");
}

TEST(TestProtocolColumnOption, getsetListPayload) {
	cql::ProtocolColumnOption value(
		cql::ProtocolColumnOptionListPayload(
			cql::ProtocolColumnOption(cql::ColumnType::Ascii)));
	ASSERT_EQ(value.get(), cql::ColumnType::List);
	auto payload = value.getListPayload();
	ASSERT_EQ(payload.getElementType().get(), cql::ColumnType::Ascii);
}

TEST(TestProtocolColumnOption, getsetMapPayload) {
	cql::ProtocolColumnOption value(
		cql::ProtocolColumnOptionMapPayload(
			cql::ProtocolColumnOption(cql::ColumnType::Int),
			cql::ProtocolColumnOption(cql::ColumnType::VarChar)));
	ASSERT_EQ(value.get(), cql::ColumnType::Map);
	auto payload = value.getMapPayload();
	ASSERT_EQ(payload.getKeyType().get(), cql::ColumnType::Int);
	ASSERT_EQ(payload.getValueType().get(), cql::ColumnType::VarChar);
}

TEST(TestProtocolColumnOption, getsetSetPayload) {
	cql::ProtocolColumnOption value(
		cql::ProtocolColumnOptionSetPayload(
			cql::ProtocolColumnOption(cql::ColumnType::Ascii)));
	ASSERT_EQ(value.get(), cql::ColumnType::Set);
	auto payload = value.getSetPayload();
	ASSERT_EQ(payload.getElementType().get(), cql::ColumnType::Ascii);
}

TEST(TestProtocolColumnOption, getsetUDTPayload) {
	cql::ProtocolColumnOption value(
		cql::ProtocolColumnOptionUDTPayload(
			cql::ProtocolString("some key space"),
			cql::ProtocolString("some udt name"),
			{
				{
					cql::ProtocolString("column a"),
					cql::ProtocolColumnOption(cql::ColumnType::Int)
				},
				{
					cql::ProtocolString("column b"),
					cql::ProtocolColumnOption(cql::ColumnType::VarChar)
				}
			}));
	ASSERT_EQ(value.get(), cql::ColumnType::UDT);
	auto payload = value.getUDTPayload();
	ASSERT_EQ(payload.getKeySpace().get(), "some key space");
	ASSERT_EQ(payload.getUDTName().get(), "some udt name");
	ASSERT_EQ(payload.getFields().size(), 2);
	ASSERT_EQ(payload.getFields().at(0).first.get(), "column a");
	ASSERT_EQ(payload.getFields().at(0).second.get(), cql::ColumnType::Int);
	ASSERT_EQ(payload.getFields().at(1).first.get(), "column b");
	ASSERT_EQ(payload.getFields().at(1).second.get(), cql::ColumnType::VarChar);
}

TEST(TestProtocolColumnOption, getsetTuplePayload) {
	cql::ProtocolColumnOption value(
		cql::ProtocolColumnOptionTuplePayload(
		{
			cql::ProtocolColumnOption(cql::ColumnType::Ascii),
			cql::ProtocolColumnOption(cql::ColumnType::Int),
			cql::ProtocolColumnOption(cql::ColumnType::BigInt)
		}));
	ASSERT_EQ(value.get(), cql::ColumnType::Tuple);
	auto payload = value.getTuplePayload();
	ASSERT_EQ(payload.getTypes().size(), 3);
	ASSERT_EQ(payload.getTypes().at(0).get(), cql::ColumnType::Ascii);
	ASSERT_EQ(payload.getTypes().at(1).get(), cql::ColumnType::Int);
	ASSERT_EQ(payload.getTypes().at(2).get(), cql::ColumnType::BigInt);
}

TEST(TestProtocolColumnOption, encode) {
	{
		// Ascii
		cql::ProtocolColumnOption value(cql::ColumnType::Ascii);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x00\x01"));
	}
	{
		// Custom
		cql::ProtocolColumnOption value(
			cql::ProtocolColumnOptionCustomPayload(
				cql::ProtocolString("abc")));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x00\x00\x00\x03""abc"));
	}
	{
		// List<Set<Int>>
		cql::ProtocolColumnOption value(
			cql::ProtocolColumnOptionListPayload(
				cql::ProtocolColumnOption(
					cql::ProtocolColumnOptionSetPayload(
						cql::ProtocolColumnOption(cql::ColumnType::Int)))));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x00\x20\x00\x22\x00\x09"));
	}
	{
		// Map<Int, VarChar>
		cql::ProtocolColumnOption value(
			cql::ProtocolColumnOptionMapPayload(
				cql::ProtocolColumnOption(cql::ColumnType::Int),
				cql::ProtocolColumnOption(cql::ColumnType::VarChar)));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x00\x21\x00\x09\x00\x0d"));
	}
	{
		// Set<Time>
		cql::ProtocolColumnOption value(
			cql::ProtocolColumnOptionSetPayload(
				cql::ProtocolColumnOption(cql::ColumnType::Time)));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x00\x22\x00\x12"));
	}
	{
		// UDT
		cql::ProtocolColumnOption value(
			cql::ProtocolColumnOptionUDTPayload(
				cql::ProtocolString("ks"),
				cql::ProtocolString("unx"),
				{
					{
						cql::ProtocolString("abc"),
						cql::ProtocolColumnOption(cql::ColumnType::Int)
					},
					{
						cql::ProtocolString("asdfg"),
						cql::ProtocolColumnOption(cql::ColumnType::VarChar)
					}
				}));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString(
			"\x00\x30"
			"\x00\x02""ks"
			"\x00\x03""unx"
			"\x00\x02"
			"\x00\x03""abc""\x00\x09"
			"\x00\x05""asdfg""\x00\x0d"));
	}
	{
		// Tuple
		cql::ProtocolColumnOption value(
			cql::ProtocolColumnOptionTuplePayload(
			{
				cql::ProtocolColumnOption(cql::ColumnType::Ascii),
				cql::ProtocolColumnOption(cql::ColumnType::Int),
			}));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, makeTestString("\x00\x31\x00\x02\x00\x01\x00\x09"));
	}
}

TEST(TestProtocolColumnOption, decode) {
	cql::ProtocolColumnOption value;
	{
		// Ascii
		auto data = makeTestString("\x00\x01");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::ColumnType::Ascii);
	}
	{
		// Custom
		auto data = makeTestString("\x00\x00\x00\x03""abc");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::ColumnType::Custom);
		ASSERT_EQ(value.getCustomPayload().get().get(), "abc");
	}
	{
		// List<Set<Int>>
		auto data = makeTestString("\x00\x20\x00\x22\x00\x09");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::ColumnType::List);
		auto payload = value.getListPayload();
		ASSERT_EQ(payload.getElementType().get(), cql::ColumnType::Set);
		ASSERT_EQ(payload.getElementType().getSetPayload().getElementType().get(), cql::ColumnType::Int);
	}
	{
		// Map<Int, VarChar>
		auto data = makeTestString("\x00\x21\x00\x09\x00\x0d");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::ColumnType::Map);
		auto payload = value.getMapPayload();
		ASSERT_EQ(payload.getKeyType().get(), cql::ColumnType::Int);
		ASSERT_EQ(payload.getValueType().get(), cql::ColumnType::VarChar);
	}
	{
		// Set<Time>
		auto data = makeTestString("\x00\x22\x00\x12");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::ColumnType::Set);
		auto payload = value.getSetPayload();
		ASSERT_EQ(payload.getElementType().get(), cql::ColumnType::Time);
	}
	{
		// UDT
		auto data = makeTestString(
			"\x00\x30"
			"\x00\x02""ks"
			"\x00\x03""unx"
			"\x00\x02"
			"\x00\x03""abc""\x00\x09"
			"\x00\x05""asdfg""\x00\x0d");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::ColumnType::UDT);
		auto payload = value.getUDTPayload();
		ASSERT_EQ(payload.getKeySpace().get(), "ks");
		ASSERT_EQ(payload.getUDTName().get(), "unx");
		ASSERT_EQ(payload.getFields().size(), 2);
		ASSERT_EQ(payload.getFields().at(0).first.get(), "abc");
		ASSERT_EQ(payload.getFields().at(0).second.get(), cql::ColumnType::Int);
		ASSERT_EQ(payload.getFields().at(1).first.get(), "asdfg");
		ASSERT_EQ(payload.getFields().at(1).second.get(), cql::ColumnType::VarChar);
	}
	{
		// Tuple
		auto data = makeTestString("\x00\x31\x00\x02\x00\x01\x00\x09");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::ColumnType::Tuple);
		auto payload = value.getTuplePayload();
		ASSERT_EQ(payload.getTypes().size(), 2);
		ASSERT_EQ(payload.getTypes().at(0).get(), cql::ColumnType::Ascii);
		ASSERT_EQ(payload.getTypes().at(1).get(), cql::ColumnType::Int);
	}
}

TEST(TestProtocolColumnOption, setPayloadError) {
	cql::ProtocolColumnOption value(cql::ColumnType::Ascii);
	ASSERT_THROWS(cql::LogicException, value.setCustomPayload(
		cql::ProtocolColumnOptionCustomPayload()));
	ASSERT_THROWS(cql::LogicException, value.setListPayload(
		cql::ProtocolColumnOptionListPayload()));
	ASSERT_THROWS(cql::LogicException, value.setMapPayload(
		cql::ProtocolColumnOptionMapPayload()));
	ASSERT_THROWS(cql::LogicException, value.setSetPayload(
		cql::ProtocolColumnOptionSetPayload()));
	ASSERT_THROWS(cql::LogicException, value.setUDTPayload(
		cql::ProtocolColumnOptionUDTPayload()));
	ASSERT_THROWS(cql::LogicException, value.setTuplePayload(
		cql::ProtocolColumnOptionTuplePayload()));
}

TEST(TestProtocolColumnOption, getPayloadError) {
	cql::ProtocolColumnOption value(cql::ColumnType::Ascii);
	ASSERT_THROWS(cql::LogicException, value.getCustomPayload());
	ASSERT_THROWS(cql::LogicException, value.getListPayload());
	ASSERT_THROWS(cql::LogicException, value.getMapPayload());
	ASSERT_THROWS(cql::LogicException, value.getSetPayload());
	ASSERT_THROWS(cql::LogicException, value.getUDTPayload());
	ASSERT_THROWS(cql::LogicException, value.getTuplePayload());
}

TEST(TestProtocolColumnOption, decodeError) {
	{
		cql::ProtocolColumnOption value;
		auto data = makeTestString("\x00");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
	{
		cql::ProtocolColumnOption value;
		auto data = makeTestString("\x00\x00\x00\x03""ab");
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::DecodeException, value.decode(ptr, end));
	}
}

