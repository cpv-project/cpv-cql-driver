#pragma once
#include <cstdint>
#include <utility>
#include <vector>
#include <core/sstring.hh>
#include <CqlDriver/Common/CqlCommonDefinitions.hpp>
#include "CqlProtocolString.hpp"

namespace cql {
	class CqlProtocolColumnOptionCustomPayload;
	class CqlProtocolColumnOptionListPayload;
	class CqlProtocolColumnOptionMapPayload;
	class CqlProtocolColumnOptionSetPayload;
	class CqlProtocolColumnOptionUdtPayload;
	class CqlProtocolColumnOptionTuplePayload;

	/**
	 * A pair of <id><value> where <id> is a [short] representing the option id,
	 * and <value> depends on that option (and can be of size 0).
	 * This is the [option] type used in query for specific column type.
	 */
	class CqlProtocolColumnOption {
	public:
		CqlColumnType get() const { return type_; }
		void set(CqlColumnType type);
		void reset();

		CqlProtocolColumnOptionCustomPayload getCustomPayload() const;
		void setCustomPayload(const CqlProtocolColumnOptionCustomPayload& payload);

		CqlProtocolColumnOptionListPayload getListPayload() const;
		void setListPayload(const CqlProtocolColumnOptionListPayload& payload);

		CqlProtocolColumnOptionMapPayload getMapPayload() const;
		void setMapPayload(const CqlProtocolColumnOptionMapPayload& payload);

		CqlProtocolColumnOptionSetPayload getSetPayload() const;
		void setSetPayload(const CqlProtocolColumnOptionSetPayload& payload);

		CqlProtocolColumnOptionUdtPayload getUdtPayload() const;
		void setUdtPayload(const CqlProtocolColumnOptionUdtPayload& payload);

		CqlProtocolColumnOptionTuplePayload getTuplePayload() const;
		void setTuplePayload(const CqlProtocolColumnOptionTuplePayload& payload);

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		CqlProtocolColumnOption() : type_(), payload_() { }
		explicit CqlProtocolColumnOption(CqlColumnType type) :
			type_(), payload_() { set(type); }
		explicit CqlProtocolColumnOption(const CqlProtocolColumnOptionCustomPayload& payload) :
			type_(), payload_() { set(cql::CqlColumnType::Custom); setCustomPayload(payload); }
		explicit CqlProtocolColumnOption(const CqlProtocolColumnOptionListPayload& payload) :
			type_(), payload_() { set(cql::CqlColumnType::List); setListPayload(payload); }
		explicit CqlProtocolColumnOption(const CqlProtocolColumnOptionMapPayload& payload) :
			type_(), payload_() { set(cql::CqlColumnType::Map); setMapPayload(payload); }
		explicit CqlProtocolColumnOption(const CqlProtocolColumnOptionSetPayload& payload) :
			type_(), payload_() { set(cql::CqlColumnType::Set); setSetPayload(payload); }
		explicit CqlProtocolColumnOption(const CqlProtocolColumnOptionUdtPayload& payload) :
			type_(), payload_() { set(cql::CqlColumnType::Udt); setUdtPayload(payload); }
		explicit CqlProtocolColumnOption(const CqlProtocolColumnOptionTuplePayload& payload) :
			type_(), payload_() { set(cql::CqlColumnType::Tuple); setTuplePayload(payload); }

	private:
		CqlColumnType type_;
		seastar::sstring payload_;
	};

	/**
	 * Payload for column type "Custom"
	 * The value is a [string].
	 */
	class CqlProtocolColumnOptionCustomPayload {
	public:
		const CqlProtocolString& get() const& { return value_; }
		CqlProtocolString& get() & { return value_; }
		void set(CqlProtocolString&& value) { value_ = std::move(value); }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		CqlProtocolColumnOptionCustomPayload() : value_() { }
		explicit CqlProtocolColumnOptionCustomPayload(CqlProtocolString&& value) :
			value_(std::move(value)) { }

	private:
		CqlProtocolString value_;
	};

	/**
	 * Payload for column type "List"
	 * The value is an [option], representing the type of the elements of the list.
	 */
	class CqlProtocolColumnOptionListPayload {
	public:
		const CqlProtocolColumnOption& getElementType() const& { return elementType_; }
		CqlProtocolColumnOption& getElementType() & { return elementType_; }
		void setElementType(CqlProtocolColumnOption&& elementType) { elementType_ = std::move(elementType); }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		CqlProtocolColumnOptionListPayload() : elementType_() { }
		explicit CqlProtocolColumnOptionListPayload(CqlProtocolColumnOption&& elementType) :
			elementType_(std::move(elementType)) { }

	private:
		CqlProtocolColumnOption elementType_;
	};

	/**
	 * Payload for column type "Map"
	 * The value is two [option], representing the types of the keys and values of the map.
	 */
	class CqlProtocolColumnOptionMapPayload {
	public:
		const CqlProtocolColumnOption& getKeyType() const& { return keyType_; }
		CqlProtocolColumnOption& getKeyType() & { return keyType_; }
		void setKeyType(CqlProtocolColumnOption&& keyType) { keyType_ = std::move(keyType); }
		const CqlProtocolColumnOption& getValueType() const& { return valueType_; }
		CqlProtocolColumnOption& getValueType() & { return valueType_; }
		void setValueType(CqlProtocolColumnOption&& valueType) { valueType_ = std::move(valueType); }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		CqlProtocolColumnOptionMapPayload() : keyType_(), valueType_() { }
		CqlProtocolColumnOptionMapPayload(
			CqlProtocolColumnOption&& keyType,
			CqlProtocolColumnOption&& valueType) :
			keyType_(std::move(keyType)),
			valueType_(std::move(valueType)) { }

	private:
		CqlProtocolColumnOption keyType_;
		CqlProtocolColumnOption valueType_;
	};

	/**
	 * Payload for column type "Set"
	 * The value is an [option], representing the type of the elements of the set.
	 */
	class CqlProtocolColumnOptionSetPayload {
	public:
		const CqlProtocolColumnOption& getElementType() const& { return elementType_; }
		CqlProtocolColumnOption& getElementType() & { return elementType_; }
		void setElementType(CqlProtocolColumnOption&& elementType) { elementType_ = std::move(elementType); }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		CqlProtocolColumnOptionSetPayload() : elementType_() { }
		explicit CqlProtocolColumnOptionSetPayload(CqlProtocolColumnOption&& elementType) :
			elementType_(std::move(elementType)) { }

	private:
		CqlProtocolColumnOption elementType_;
	};

	/**
	 * Payload for column type "Udt"
	 * The value is <ks><udt_name><n><name_1><type_1>...<name_n><type_n> where:
	 * - <ks> is a [string] representing the keyspace name this UDT is part of
	 * - <udt_name> is a [string] representing the UDT name
	 * - <n> is a [short] representing the number of fields of the UDT,
	 *   and thus the number of <name_i><type_i> pairs following
	 * - <name_i> is a [string] representing the name of the i_th field of the UDT
	 * - <type_i> is an [option] representing the type of the i_th field of the UDT
	 */
	class CqlProtocolColumnOptionUdtPayload {
	public:
		using FieldsType = std::vector<std::pair<CqlProtocolString, CqlProtocolColumnOption>>;
		const CqlProtocolString& getKeySpace() const& { return keySpace_; }
		CqlProtocolString& getKeySpace() & { return keySpace_; }
		void setKeySpace(CqlProtocolString&& keySpace) { keySpace_ = std::move(keySpace); }
		const CqlProtocolString& getUdtName() const& { return udtName_; }
		CqlProtocolString& getUdtName() & { return udtName_; }
		void setUdtName(CqlProtocolString&& udtName) { udtName_ = std::move(udtName); }
		const FieldsType& getFields() const& { return fields_; }
		FieldsType& getFields() & { return fields_; }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		CqlProtocolColumnOptionUdtPayload() : keySpace_(), udtName_(), fields_() { }
		CqlProtocolColumnOptionUdtPayload(
			CqlProtocolString&& keySpace,
			CqlProtocolString&& udtName,
			FieldsType&& fields) :
			keySpace_(std::move(keySpace)),
			udtName_(std::move(udtName)),
			fields_(std::move(fields)) { }

	private:
		CqlProtocolString keySpace_;
		CqlProtocolString udtName_;
		FieldsType fields_;
	};

	/**
	 * Payload for column type "Tuple"
	 * The value is <n><type_1>...<type_n> where <n> is a [short] representing the number of values in the type
	 * and <type_i> are [option] representing the type of i_th component of the tuple
	 */
	class CqlProtocolColumnOptionTuplePayload {
	public:
		using TypesType = std::vector<CqlProtocolColumnOption>;
		const TypesType& getTypes() const& { return types_; }
		TypesType& getTypes() & { return types_; }

		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		CqlProtocolColumnOptionTuplePayload() : types_() { }
		explicit CqlProtocolColumnOptionTuplePayload(TypesType&& types) :
			types_(std::move(types)) { }

	private:
		TypesType types_;
	};
}

