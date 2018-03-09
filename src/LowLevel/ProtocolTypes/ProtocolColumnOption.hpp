#pragma once
#include <cstdint>
#include <utility>
#include <vector>
#include <core/sstring.hh>
#include <CQLDriver/Common/CommonDefinitions.hpp>
#include "./ProtocolString.hpp"

namespace cql {
	class ProtocolColumnOptionCustomPayload;
	class ProtocolColumnOptionListPayload;
	class ProtocolColumnOptionMapPayload;
	class ProtocolColumnOptionSetPayload;
	class ProtocolColumnOptionUDTPayload;
	class ProtocolColumnOptionTuplePayload;

	/**
	 * A pair of <id><value> where <id> is a [short] representing the option id,
	 * and <value> depends on that option (and can be of size 0).
	 * This is the [option] type used in query for specific column type.
	 */
	class ProtocolColumnOption {
	public:
		/** Get the column type */
		ColumnType get() const { return type_; }

		/** Set the column type */
		void set(ColumnType type);

		/** Reset to initial state */
		void reset();

		/** Payload of custom column type */
		ProtocolColumnOptionCustomPayload getCustomPayload() const;
		void setCustomPayload(const ProtocolColumnOptionCustomPayload& payload);

		/** Payload of list column type (element type) */
		ProtocolColumnOptionListPayload getListPayload() const;
		void setListPayload(const ProtocolColumnOptionListPayload& payload);

		/** Payload of map column type (key and value type) */
		ProtocolColumnOptionMapPayload getMapPayload() const;
		void setMapPayload(const ProtocolColumnOptionMapPayload& payload);

		/** Payload of set column type (element type) */
		ProtocolColumnOptionSetPayload getSetPayload() const;
		void setSetPayload(const ProtocolColumnOptionSetPayload& payload);

		/** Payload of udt column type (name and type of fields) */
		ProtocolColumnOptionUDTPayload getUDTPayload() const;
		void setUDTPayload(const ProtocolColumnOptionUDTPayload& payload);

		/** Payload of tuple column type (types) */
		ProtocolColumnOptionTuplePayload getTuplePayload() const;
		void setTuplePayload(const ProtocolColumnOptionTuplePayload& payload);

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructors */
		ProtocolColumnOption() : type_(), payload_() { }
		explicit ProtocolColumnOption(ColumnType type) :
			type_(), payload_() { set(type); }
		explicit ProtocolColumnOption(const ProtocolColumnOptionCustomPayload& payload) :
			type_(), payload_() { set(cql::ColumnType::Custom); setCustomPayload(payload); }
		explicit ProtocolColumnOption(const ProtocolColumnOptionListPayload& payload) :
			type_(), payload_() { set(cql::ColumnType::List); setListPayload(payload); }
		explicit ProtocolColumnOption(const ProtocolColumnOptionMapPayload& payload) :
			type_(), payload_() { set(cql::ColumnType::Map); setMapPayload(payload); }
		explicit ProtocolColumnOption(const ProtocolColumnOptionSetPayload& payload) :
			type_(), payload_() { set(cql::ColumnType::Set); setSetPayload(payload); }
		explicit ProtocolColumnOption(const ProtocolColumnOptionUDTPayload& payload) :
			type_(), payload_() { set(cql::ColumnType::UDT); setUDTPayload(payload); }
		explicit ProtocolColumnOption(const ProtocolColumnOptionTuplePayload& payload) :
			type_(), payload_() { set(cql::ColumnType::Tuple); setTuplePayload(payload); }

	private:
		ColumnType type_;
		seastar::sstring payload_;
	};

	/**
	 * Payload for column type "Custom"
	 * The value is a [string].
	 */
	class ProtocolColumnOptionCustomPayload {
	public:
		/** The custom data */
		const ProtocolString& get() const& { return value_; }
		ProtocolString& get() & { return value_; }
		void set(ProtocolString&& value) { value_ = std::move(value); }

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructors */
		ProtocolColumnOptionCustomPayload() : value_() { }
		explicit ProtocolColumnOptionCustomPayload(ProtocolString&& value) :
			value_(std::move(value)) { }

	private:
		ProtocolString value_;
	};

	/**
	 * Payload for column type "List"
	 * The value is an [option], representing the type of the elements of the list.
	 */
	class ProtocolColumnOptionListPayload {
	public:
		/** The element type of list */
		const ProtocolColumnOption& getElementType() const& { return elementType_; }
		ProtocolColumnOption& getElementType() & { return elementType_; }
		void setElementType(ProtocolColumnOption&& elementType) { elementType_ = std::move(elementType); }

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructors */
		ProtocolColumnOptionListPayload() : elementType_() { }
		explicit ProtocolColumnOptionListPayload(ProtocolColumnOption&& elementType) :
			elementType_(std::move(elementType)) { }

	private:
		ProtocolColumnOption elementType_;
	};

	/**
	 * Payload for column type "Map"
	 * The value is two [option], representing the types of the keys and values of the map.
	 */
	class ProtocolColumnOptionMapPayload {
	public:
		/** The key type of map */
		const ProtocolColumnOption& getKeyType() const& { return keyType_; }
		ProtocolColumnOption& getKeyType() & { return keyType_; }
		void setKeyType(ProtocolColumnOption&& keyType) { keyType_ = std::move(keyType); }

		/** The value type of map */
		const ProtocolColumnOption& getValueType() const& { return valueType_; }
		ProtocolColumnOption& getValueType() & { return valueType_; }
		void setValueType(ProtocolColumnOption&& valueType) { valueType_ = std::move(valueType); }

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructors */
		ProtocolColumnOptionMapPayload() : keyType_(), valueType_() { }
		ProtocolColumnOptionMapPayload(
			ProtocolColumnOption&& keyType,
			ProtocolColumnOption&& valueType) :
			keyType_(std::move(keyType)),
			valueType_(std::move(valueType)) { }

	private:
		ProtocolColumnOption keyType_;
		ProtocolColumnOption valueType_;
	};

	/**
	 * Payload for column type "Set"
	 * The value is an [option], representing the type of the elements of the set.
	 */
	class ProtocolColumnOptionSetPayload {
	public:
		/** The element type of set */
		const ProtocolColumnOption& getElementType() const& { return elementType_; }
		ProtocolColumnOption& getElementType() & { return elementType_; }
		void setElementType(ProtocolColumnOption&& elementType) { elementType_ = std::move(elementType); }

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructors */
		ProtocolColumnOptionSetPayload() : elementType_() { }
		explicit ProtocolColumnOptionSetPayload(ProtocolColumnOption&& elementType) :
			elementType_(std::move(elementType)) { }

	private:
		ProtocolColumnOption elementType_;
	};

	/**
	 * Payload for column type "UDT" (user defined type)
	 * The value is <ks><udt_name><n><name_1><type_1>...<name_n><type_n> where:
	 * - <ks> is a [string] representing the keyspace name this UDT is part of
	 * - <udt_name> is a [string] representing the UDT name
	 * - <n> is a [short] representing the number of fields of the UDT,
	 *   and thus the number of <name_i><type_i> pairs following
	 * - <name_i> is a [string] representing the name of the i_th field of the UDT
	 * - <type_i> is an [option] representing the type of the i_th field of the UDT
	 */
	class ProtocolColumnOptionUDTPayload {
	public:
		using FieldsType = std::vector<std::pair<ProtocolString, ProtocolColumnOption>>;

		/** The key space */
		const ProtocolString& getKeySpace() const& { return keySpace_; }
		ProtocolString& getKeySpace() & { return keySpace_; }
		void setKeySpace(ProtocolString&& keySpace) { keySpace_ = std::move(keySpace); }

		/** The user defined type name */
		const ProtocolString& getUDTName() const& { return udtName_; }
		ProtocolString& getUDTName() & { return udtName_; }
		void setUDTName(ProtocolString&& udtName) { udtName_ = std::move(udtName); }

		/** The fields of user defined type */
		const FieldsType& getFields() const& { return fields_; }
		FieldsType& getFields() & { return fields_; }

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructors */
		ProtocolColumnOptionUDTPayload() : keySpace_(), udtName_(), fields_() { }
		ProtocolColumnOptionUDTPayload(
			ProtocolString&& keySpace,
			ProtocolString&& udtName,
			FieldsType&& fields) :
			keySpace_(std::move(keySpace)),
			udtName_(std::move(udtName)),
			fields_(std::move(fields)) { }

	private:
		ProtocolString keySpace_;
		ProtocolString udtName_;
		FieldsType fields_;
	};

	/**
	 * Payload for column type "Tuple"
	 * The value is <n><type_1>...<type_n> where <n> is a [short] representing the number of values in the type
	 * and <type_i> are [option] representing the type of i_th component of the tuple
	 */
	class ProtocolColumnOptionTuplePayload {
	public:
		using TypesType = std::vector<ProtocolColumnOption>;

		/** The types of tuple */
		const TypesType& getTypes() const& { return types_; }
		TypesType& getTypes() & { return types_; }

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructors */
		ProtocolColumnOptionTuplePayload() : types_() { }
		explicit ProtocolColumnOptionTuplePayload(TypesType&& types) :
			types_(std::move(types)) { }

	private:
		TypesType types_;
	};
}

