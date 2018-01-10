#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <LowLevel/ProtocolTypes/CqlProtocolColumnOption.hpp>
#include <TestUtility/GTestUtils.hpp>

TEST(TestCqlProtocolColumnOption, getset) {
	cql::CqlProtocolColumnOption value(cql::CqlColumnType::Ascii);
	ASSERT_EQ(value.get(), cql::CqlColumnType::Ascii);
	value.set(cql::CqlColumnType::Int);
	ASSERT_EQ(value.get(), cql::CqlColumnType::Int);

	value = cql::CqlProtocolColumnOption();
	ASSERT_EQ(value.get(), cql::CqlColumnType::Custom);
}

TEST(TestCqlProtocolColumnOption, getsetCustomPayload) {
	cql::CqlProtocolColumnOption value(
		cql::CqlProtocolColumnOptionCustomPayload(
			cql::CqlProtocolString("abc")));
	ASSERT_EQ(value.get(), cql::CqlColumnType::Custom);
	auto payload = value.getCustomPayload();
	ASSERT_EQ(payload.get().get(), "abc");
}

TEST(TestCqlProtocolColumnOption, getsetListPayload) {
	cql::CqlProtocolColumnOption value(
		cql::CqlProtocolColumnOptionListPayload(
			cql::CqlProtocolColumnOption(cql::CqlColumnType::Ascii)));
	ASSERT_EQ(value.get(), cql::CqlColumnType::List);
	auto payload = value.getListPayload();
	ASSERT_EQ(payload.getElementType().get(), cql::CqlColumnType::Ascii);
}

TEST(TestCqlProtocolColumnOption, getsetMapPayload) {
	cql::CqlProtocolColumnOption value(
		cql::CqlProtocolColumnOptionMapPayload(
			cql::CqlProtocolColumnOption(cql::CqlColumnType::Int),
			cql::CqlProtocolColumnOption(cql::CqlColumnType::VarChar)));
	ASSERT_EQ(value.get(), cql::CqlColumnType::Map);
	auto payload = value.getMapPayload();
	ASSERT_EQ(payload.getKeyType().get(), cql::CqlColumnType::Int);
	ASSERT_EQ(payload.getValueType().get(), cql::CqlColumnType::VarChar);
}

TEST(TestCqlProtocolColumnOption, getsetSetPayload) {
	cql::CqlProtocolColumnOption value(
		cql::CqlProtocolColumnOptionSetPayload(
			cql::CqlProtocolColumnOption(cql::CqlColumnType::Ascii)));
	ASSERT_EQ(value.get(), cql::CqlColumnType::Set);
	auto payload = value.getSetPayload();
	ASSERT_EQ(payload.getElementType().get(), cql::CqlColumnType::Ascii);
}

TEST(TestCqlProtocolColumnOption, getsetUdtPayload) {
	cql::CqlProtocolColumnOption value(
		cql::CqlProtocolColumnOptionUdtPayload(
			cql::CqlProtocolString("some key space"),
			cql::CqlProtocolString("some udt name"),
			{
				{
					cql::CqlProtocolString("column a"),
					cql::CqlProtocolColumnOption(cql::CqlColumnType::Int)
				},
				{
					cql::CqlProtocolString("column b"),
					cql::CqlProtocolColumnOption(cql::CqlColumnType::VarChar)
				}
			}));
	ASSERT_EQ(value.get(), cql::CqlColumnType::Udt);
	auto payload = value.getUdtPayload();
	ASSERT_EQ(payload.getKeySpace().get(), "some key space");
	ASSERT_EQ(payload.getUdtName().get(), "some udt name");
	ASSERT_EQ(payload.getFields().size(), 2);
	ASSERT_EQ(payload.getFields().at(0).first.get(), "column a");
	ASSERT_EQ(payload.getFields().at(0).second.get(), cql::CqlColumnType::Int);
	ASSERT_EQ(payload.getFields().at(1).first.get(), "column b");
	ASSERT_EQ(payload.getFields().at(1).second.get(), cql::CqlColumnType::VarChar);
}

TEST(TestCqlProtocolColumnOption, getsetTuplePayload) {
	cql::CqlProtocolColumnOption value(
		cql::CqlProtocolColumnOptionTuplePayload(
		{
			cql::CqlProtocolColumnOption(cql::CqlColumnType::Ascii),
			cql::CqlProtocolColumnOption(cql::CqlColumnType::Int),
			cql::CqlProtocolColumnOption(cql::CqlColumnType::BigInt)
		}));
	ASSERT_EQ(value.get(), cql::CqlColumnType::Tuple);
	auto payload = value.getTuplePayload();
	ASSERT_EQ(payload.getTypes().size(), 3);
	ASSERT_EQ(payload.getTypes().at(0).get(), cql::CqlColumnType::Ascii);
	ASSERT_EQ(payload.getTypes().at(1).get(), cql::CqlColumnType::Int);
	ASSERT_EQ(payload.getTypes().at(2).get(), cql::CqlColumnType::BigInt);
}

TEST(TestCqlProtocolColumnOption, encode) {
	{
		// Ascii
		cql::CqlProtocolColumnOption value(cql::CqlColumnType::Ascii);
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\x00\x01", 2));
	}
	{
		// Custom
		cql::CqlProtocolColumnOption value(
			cql::CqlProtocolColumnOptionCustomPayload(
				cql::CqlProtocolString("abc")));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\x00\x00\x00\x03""abc", 7));
	}
	{
		// List<Set<Int>>
		cql::CqlProtocolColumnOption value(
			cql::CqlProtocolColumnOptionListPayload(
				cql::CqlProtocolColumnOption(
					cql::CqlProtocolColumnOptionSetPayload(
						cql::CqlProtocolColumnOption(cql::CqlColumnType::Int)))));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\x00\x20\x00\x22\x00\x09", 6));
	}
	{
		// Map<Int, VarChar>
		cql::CqlProtocolColumnOption value(
			cql::CqlProtocolColumnOptionMapPayload(
				cql::CqlProtocolColumnOption(cql::CqlColumnType::Int),
				cql::CqlProtocolColumnOption(cql::CqlColumnType::VarChar)));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\x00\x21\x00\x09\x00\x0d", 6));
	}
	{
		// Set<Time>
		cql::CqlProtocolColumnOption value(
			cql::CqlProtocolColumnOptionSetPayload(
				cql::CqlProtocolColumnOption(cql::CqlColumnType::Time)));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\x00\x22\x00\x12", 4));
	}
	{
		// Udt
		cql::CqlProtocolColumnOption value(
			cql::CqlProtocolColumnOptionUdtPayload(
				cql::CqlProtocolString("ks"),
				cql::CqlProtocolString("unx"),
				{
					{
						cql::CqlProtocolString("abc"),
						cql::CqlProtocolColumnOption(cql::CqlColumnType::Int)
					},
					{
						cql::CqlProtocolString("asdfg"),
						cql::CqlProtocolColumnOption(cql::CqlColumnType::VarChar)
					}
				}));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring(
			"\x00\x30"
			"\x00\x02""ks"
			"\x00\x03""unx"
			"\x00\x02"
			"\x00\x03""abc""\x00\x09"
			"\x00\x05""asdfg""\x00\x0d", 29));
	}
	{
		// Tuple
		cql::CqlProtocolColumnOption value(
			cql::CqlProtocolColumnOptionTuplePayload(
			{
				cql::CqlProtocolColumnOption(cql::CqlColumnType::Ascii),
				cql::CqlProtocolColumnOption(cql::CqlColumnType::Int),
			}));
		seastar::sstring data;
		value.encode(data);
		ASSERT_EQ(data, seastar::sstring("\x00\x31\x00\x02\x00\x01\x00\x09", 8));
	}
}

TEST(TestCqlProtocolColumnOption, decode) {
	cql::CqlProtocolColumnOption value;
	{
		// Ascii
		seastar::sstring data("\x00\x01", 2);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::CqlColumnType::Ascii);
	}
	{
		// Custom
		seastar::sstring data("\x00\x00\x00\x03""abc", 7);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::CqlColumnType::Custom);
		ASSERT_EQ(value.getCustomPayload().get().get(), "abc");
	}
	{
		// List<Set<Int>>
		seastar::sstring data("\x00\x20\x00\x22\x00\x09", 6);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::CqlColumnType::List);
		auto payload = value.getListPayload();
		ASSERT_EQ(payload.getElementType().get(), cql::CqlColumnType::Set);
		ASSERT_EQ(payload.getElementType().getSetPayload().getElementType().get(), cql::CqlColumnType::Int);
	}
	{
		// Map<Int, VarChar>
		seastar::sstring data("\x00\x21\x00\x09\x00\x0d", 6);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::CqlColumnType::Map);
		auto payload = value.getMapPayload();
		ASSERT_EQ(payload.getKeyType().get(), cql::CqlColumnType::Int);
		ASSERT_EQ(payload.getValueType().get(), cql::CqlColumnType::VarChar);
	}
	{
		// Set<Time>
		seastar::sstring data("\x00\x22\x00\x12", 4);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::CqlColumnType::Set);
		auto payload = value.getSetPayload();
		ASSERT_EQ(payload.getElementType().get(), cql::CqlColumnType::Time);
	}
	{
		// Udt
		seastar::sstring data(
			"\x00\x30"
			"\x00\x02""ks"
			"\x00\x03""unx"
			"\x00\x02"
			"\x00\x03""abc""\x00\x09"
			"\x00\x05""asdfg""\x00\x0d", 29);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::CqlColumnType::Udt);
		auto payload = value.getUdtPayload();
		ASSERT_EQ(payload.getKeySpace().get(), "ks");
		ASSERT_EQ(payload.getUdtName().get(), "unx");
		ASSERT_EQ(payload.getFields().size(), 2);
		ASSERT_EQ(payload.getFields().at(0).first.get(), "abc");
		ASSERT_EQ(payload.getFields().at(0).second.get(), cql::CqlColumnType::Int);
		ASSERT_EQ(payload.getFields().at(1).first.get(), "asdfg");
		ASSERT_EQ(payload.getFields().at(1).second.get(), cql::CqlColumnType::VarChar);
	}
	{
		// Tuple
		seastar::sstring data("\x00\x31\x00\x02\x00\x01\x00\x09", 8);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		value.decode(ptr, end);
		ASSERT_TRUE(ptr == end);
		ASSERT_EQ(value.get(), cql::CqlColumnType::Tuple);
		auto payload = value.getTuplePayload();
		ASSERT_EQ(payload.getTypes().size(), 2);
		ASSERT_EQ(payload.getTypes().at(0).get(), cql::CqlColumnType::Ascii);
		ASSERT_EQ(payload.getTypes().at(1).get(), cql::CqlColumnType::Int);
	}
}

TEST(TestCqlProtocolColumnOption, setPayloadError) {
	cql::CqlProtocolColumnOption value(cql::CqlColumnType::Ascii);
	ASSERT_THROWS(cql::CqlLogicException, value.setCustomPayload(
		cql::CqlProtocolColumnOptionCustomPayload()));
	ASSERT_THROWS(cql::CqlLogicException, value.setListPayload(
		cql::CqlProtocolColumnOptionListPayload()));
	ASSERT_THROWS(cql::CqlLogicException, value.setMapPayload(
		cql::CqlProtocolColumnOptionMapPayload()));
	ASSERT_THROWS(cql::CqlLogicException, value.setSetPayload(
		cql::CqlProtocolColumnOptionSetPayload()));
	ASSERT_THROWS(cql::CqlLogicException, value.setUdtPayload(
		cql::CqlProtocolColumnOptionUdtPayload()));
	ASSERT_THROWS(cql::CqlLogicException, value.setTuplePayload(
		cql::CqlProtocolColumnOptionTuplePayload()));
}

TEST(TestCqlProtocolColumnOption, getPayloadError) {
	cql::CqlProtocolColumnOption value(cql::CqlColumnType::Ascii);
	ASSERT_THROWS(cql::CqlLogicException, value.getCustomPayload());
	ASSERT_THROWS(cql::CqlLogicException, value.getListPayload());
	ASSERT_THROWS(cql::CqlLogicException, value.getMapPayload());
	ASSERT_THROWS(cql::CqlLogicException, value.getSetPayload());
	ASSERT_THROWS(cql::CqlLogicException, value.getUdtPayload());
	ASSERT_THROWS(cql::CqlLogicException, value.getTuplePayload());
}

TEST(TestCqlProtocolColumnOption, decodeError) {
	{
		cql::CqlProtocolColumnOption value;
		seastar::sstring data("\x00", 1);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
	{
		cql::CqlProtocolColumnOption value;
		seastar::sstring data("\x00\x00\x00\x03""ab", 6);
		auto ptr = data.c_str();
		auto end = ptr + data.size();
		ASSERT_THROWS(cql::CqlDecodeException, value.decode(ptr, end));
	}
}

