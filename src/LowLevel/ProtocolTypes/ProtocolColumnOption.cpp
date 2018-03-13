#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <CQLDriver/Common/Exceptions/EncodeException.hpp>
#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include "./ProtocolColumnOption.hpp"
#include "./ProtocolShort.hpp"

namespace cql {
	/** Set the column type */
	void ProtocolColumnOption::set(ColumnType type) {
		type_ = type;
		payload_.resize(0); // should set type first then payload
	}

	/** Reset to initial state */
	void ProtocolColumnOption::reset() {
		set(ColumnType::Custom);
	}

	/** Get the payload of custom column type */
	ProtocolColumnOptionCustomPayload ProtocolColumnOption::getCustomPayload() const {
		if (type_ != ColumnType::Custom) {
			throw LogicException(CQL_CODEINFO, "type not matched");
		}
		ProtocolColumnOptionCustomPayload payload;
		const char* ptr = payload_.c_str();
		payload.decode(ptr, ptr + payload_.size());
		return payload;
	}

	/** Set the payload of custom column type */
	void ProtocolColumnOption::setCustomPayload(const ProtocolColumnOptionCustomPayload& payload) {
		if (type_ != ColumnType::Custom) {
			throw LogicException(CQL_CODEINFO, "type not matched");
		}
		payload_.resize(0);
		payload.encode(payload_);
	}

	/** Get the payload of list column type */
	ProtocolColumnOptionListPayload ProtocolColumnOption::getListPayload() const {
		if (type_ != ColumnType::List) {
			throw LogicException(CQL_CODEINFO, "type not matched");
		}
		ProtocolColumnOptionListPayload payload;
		const char* ptr = payload_.c_str();
		payload.decode(ptr, ptr + payload_.size());
		return payload;
	}

	/** Set the payload of list column type */
	void ProtocolColumnOption::setListPayload(const ProtocolColumnOptionListPayload& payload) {
		if (type_ != ColumnType::List) {
			throw LogicException(CQL_CODEINFO, "type not matched");
		}
		payload_.resize(0);
		payload.encode(payload_);
	}

	/** Get the payload of map column type */
	ProtocolColumnOptionMapPayload ProtocolColumnOption::getMapPayload() const {
		if (type_ != ColumnType::Map) {
			throw LogicException(CQL_CODEINFO, "type not matched");
		}
		ProtocolColumnOptionMapPayload payload;
		const char* ptr = payload_.c_str();
		payload.decode(ptr, ptr + payload_.size());
		return payload;
	}

	/** Set the payload of map column type */
	void ProtocolColumnOption::setMapPayload(const ProtocolColumnOptionMapPayload& payload) {
		if (type_ != ColumnType::Map) {
			throw LogicException(CQL_CODEINFO, "type not matched");
		}
		payload_.resize(0);
		payload.encode(payload_);
	}

	/** Get the payload of set column type */
	ProtocolColumnOptionSetPayload ProtocolColumnOption::getSetPayload() const {
		if (type_ != ColumnType::Set) {
			throw LogicException(CQL_CODEINFO, "type not matched");
		}
		ProtocolColumnOptionSetPayload payload;
		const char* ptr = payload_.c_str();
		payload.decode(ptr, ptr + payload_.size());
		return payload;
	}

	/** Set the payload of set column type */
	void ProtocolColumnOption::setSetPayload(const ProtocolColumnOptionSetPayload& payload) {
		if (type_ != ColumnType::Set) {
			throw LogicException(CQL_CODEINFO, "type not matched");
		}
		payload_.resize(0);
		payload.encode(payload_);
	}

	/** Get the payload of UDT column type */
	ProtocolColumnOptionUDTPayload ProtocolColumnOption::getUDTPayload() const {
		if (type_ != ColumnType::UDT) {
			throw LogicException(CQL_CODEINFO, "type not matched");
		}
		ProtocolColumnOptionUDTPayload payload;
		const char* ptr = payload_.c_str();
		payload.decode(ptr, ptr + payload_.size());
		return payload;
	}

	/** Set the payload of UDT column type */
	void ProtocolColumnOption::setUDTPayload(const ProtocolColumnOptionUDTPayload& payload) {
		if (type_ != ColumnType::UDT) {
			throw LogicException(CQL_CODEINFO, "type not matched");
		}
		payload_.resize(0);
		payload.encode(payload_);
	}

	/** Get the payload of tuple column type */
	ProtocolColumnOptionTuplePayload ProtocolColumnOption::getTuplePayload() const {
		if (type_ != ColumnType::Tuple) {
			throw LogicException(CQL_CODEINFO, "type not matched");
		}
		ProtocolColumnOptionTuplePayload payload;
		const char* ptr = payload_.c_str();
		payload.decode(ptr, ptr + payload_.size());
		return payload;
	}

	/** Set the payloda of tuple column type */
	void ProtocolColumnOption::setTuplePayload(const ProtocolColumnOptionTuplePayload& payload) {
		if (type_ != ColumnType::Tuple) {
			throw LogicException(CQL_CODEINFO, "type not matched");
		}
		payload_.resize(0);
		payload.encode(payload_);
	}

	/** Encode to binary data */
	void ProtocolColumnOption::encode(std::string& data) const {
		if ((type_ == ColumnType::Custom ||
			type_ == ColumnType::List ||
			type_ == ColumnType::Map ||
			type_ == ColumnType::Set ||
			type_ == ColumnType::UDT ||
			type_ == ColumnType::Tuple) &&
			payload_.empty()) {
			throw EncodeException(CQL_CODEINFO, "this column type require payload presented");
		}
		ProtocolShort type(static_cast<std::uint16_t>(type_));
		type.encode(data);
		data.append(payload_.c_str(), payload_.size());
	}

	/** Decode from binary data */
	void ProtocolColumnOption::decode(const char*& ptr, const char* end) {
		ProtocolShort type;
		type.decode(ptr, end);
		type_ = static_cast<ColumnType>(type.get());
		payload_.resize(0);
		if (type_ == ColumnType::Custom) {
			static thread_local ProtocolString payload;
			payload.decode(ptr, end);
			payload.encode(payload_);
		} else if (type_ == ColumnType::List) {
			static thread_local ProtocolColumnOptionListPayload payload;
			payload.decode(ptr, end);
			payload.encode(payload_);
		} else if (type_ == ColumnType::Map) {
			static thread_local ProtocolColumnOptionMapPayload payload;
			payload.decode(ptr, end);
			payload.encode(payload_);
		} else if (type_ == ColumnType::Set) {
			static thread_local ProtocolColumnOptionSetPayload payload;
			payload.decode(ptr, end);
			payload.encode(payload_);
		} else if (type_ == ColumnType::UDT) {
			static thread_local ProtocolColumnOptionUDTPayload payload;
			payload.decode(ptr, end);
			payload.encode(payload_);
		} else if (type_ == ColumnType::Tuple) {
			static thread_local ProtocolColumnOptionTuplePayload payload;
			payload.decode(ptr, end);
			payload.encode(payload_);
		}
	}

	/** Encode to binary data */
	void ProtocolColumnOptionCustomPayload::encode(std::string& data) const {
		value_.encode(data);
	}

	/** Decode from binary data */
	void ProtocolColumnOptionCustomPayload::decode(const char*& ptr, const char* end) {
		value_.decode(ptr, end);
	}

	/** Encode to binary data */
	void ProtocolColumnOptionListPayload::encode(std::string& data) const {
		elementType_.encode(data);
	}

	/** Decode from binary data */
	void ProtocolColumnOptionListPayload::decode(const char*& ptr, const char* end) {
		elementType_.decode(ptr, end);
	}

	/** Encode to binary data */
	void ProtocolColumnOptionMapPayload::encode(std::string& data) const {
		keyType_.encode(data);
		valueType_.encode(data);
	}

	/** Decode from binary data */
	void ProtocolColumnOptionMapPayload::decode(const char*& ptr, const char* end) {
		keyType_.decode(ptr, end);
		valueType_.decode(ptr, end);
	}

	/** Encode to binary data */
	void ProtocolColumnOptionSetPayload::encode(std::string& data) const {
		elementType_.encode(data);
	}

	/** Decode from binary data */
	void ProtocolColumnOptionSetPayload::decode(const char*& ptr, const char* end) {
		elementType_.decode(ptr, end);
	}

	/** Encode to binary data */
	void ProtocolColumnOptionUDTPayload::encode(std::string& data) const {
		ProtocolShort fieldsLength(fields_.size());
		if (fieldsLength.get() != fields_.size()) {
			throw LogicException(CQL_CODEINFO, "too many udt fields cause overflow");
		}
		keySpace_.encode(data);
		udtName_.encode(data);
		fieldsLength.encode(data);
		for (const auto& pair : fields_) {
			pair.first.encode(data);
			pair.second.encode(data);
		}
	}

	/** Decode from binary data */
	void ProtocolColumnOptionUDTPayload::decode(const char*& ptr, const char* end) {
		ProtocolShort fieldsLength;
		keySpace_.decode(ptr, end);
		udtName_.decode(ptr, end);
		fieldsLength.decode(ptr, end);
		fields_.clear();
		for (std::size_t i = 0; i < fieldsLength.get(); ++i) {
			fields_.emplace_back();
			fields_.back().first.decode(ptr, end);
			fields_.back().second.decode(ptr, end);
		}
	}

	/** Encode to binary data */
	void ProtocolColumnOptionTuplePayload::encode(std::string& data) const {
		ProtocolShort fieldsLength(types_.size());
		if (fieldsLength.get() != types_.size()) {
			throw LogicException(CQL_CODEINFO, "too many tuple types cause overflow");
		}
		fieldsLength.encode(data);
		for (const auto& type : types_) {
			type.encode(data);
		}
	}

	/** Decode from binary data */
	void ProtocolColumnOptionTuplePayload::decode(const char*& ptr, const char* end) {
		ProtocolShort fieldsLength;
		fieldsLength.decode(ptr, end);
		types_.clear();
		for (std::size_t i = 0; i < fieldsLength.get(); ++i) {
			types_.emplace_back();
			types_.back().decode(ptr, end);
		}
	}
}

