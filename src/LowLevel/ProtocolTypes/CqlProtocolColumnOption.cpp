#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include <CqlDriver/Common/Exceptions/CqlEncodeException.hpp>
#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include "CqlProtocolColumnOption.hpp"
#include "CqlProtocolShort.hpp"

namespace cql {
	void CqlProtocolColumnOption::set(CqlColumnType type) {
		type_ = type;
		payload_.resize(0); // should set type first then payload
	}

	CqlProtocolColumnOptionCustomPayload CqlProtocolColumnOption::getCustomPayload() const {
		if (type_ != CqlColumnType::Custom) {
			throw CqlLogicException(CQL_CODEINFO, "type not matched");
		}
		CqlProtocolColumnOptionCustomPayload payload;
		const char* ptr = payload_.c_str();
		payload.decode(ptr, ptr + payload_.size());
		return payload;
	}

	void CqlProtocolColumnOption::setCustomPayload(const CqlProtocolColumnOptionCustomPayload& payload) {
		if (type_ != CqlColumnType::Custom) {
			throw CqlLogicException(CQL_CODEINFO, "type not matched");
		}
		payload_.resize(0);
		payload.encode(payload_);
	}

	CqlProtocolColumnOptionListPayload CqlProtocolColumnOption::getListPayload() const {
		if (type_ != CqlColumnType::List) {
			throw CqlLogicException(CQL_CODEINFO, "type not matched");
		}
		CqlProtocolColumnOptionListPayload payload;
		const char* ptr = payload_.c_str();
		payload.decode(ptr, ptr + payload_.size());
		return payload;
	}

	void CqlProtocolColumnOption::setListPayload(const CqlProtocolColumnOptionListPayload& payload) {
		if (type_ != CqlColumnType::List) {
			throw CqlLogicException(CQL_CODEINFO, "type not matched");
		}
		payload_.resize(0);
		payload.encode(payload_);
	}

	CqlProtocolColumnOptionMapPayload CqlProtocolColumnOption::getMapPayload() const {
		if (type_ != CqlColumnType::Map) {
			throw CqlLogicException(CQL_CODEINFO, "type not matched");
		}
		CqlProtocolColumnOptionMapPayload payload;
		const char* ptr = payload_.c_str();
		payload.decode(ptr, ptr + payload_.size());
		return payload;
	}

	void CqlProtocolColumnOption::setMapPayload(const CqlProtocolColumnOptionMapPayload& payload) {
		if (type_ != CqlColumnType::Map) {
			throw CqlLogicException(CQL_CODEINFO, "type not matched");
		}
		payload_.resize(0);
		payload.encode(payload_);
	}

	CqlProtocolColumnOptionSetPayload CqlProtocolColumnOption::getSetPayload() const {
		if (type_ != CqlColumnType::Set) {
			throw CqlLogicException(CQL_CODEINFO, "type not matched");
		}
		CqlProtocolColumnOptionSetPayload payload;
		const char* ptr = payload_.c_str();
		payload.decode(ptr, ptr + payload_.size());
		return payload;
	}

	void CqlProtocolColumnOption::setSetPayload(const CqlProtocolColumnOptionSetPayload& payload) {
		if (type_ != CqlColumnType::Set) {
			throw CqlLogicException(CQL_CODEINFO, "type not matched");
		}
		payload_.resize(0);
		payload.encode(payload_);
	}

	CqlProtocolColumnOptionUdtPayload CqlProtocolColumnOption::getUdtPayload() const {
		if (type_ != CqlColumnType::Udt) {
			throw CqlLogicException(CQL_CODEINFO, "type not matched");
		}
		CqlProtocolColumnOptionUdtPayload payload;
		const char* ptr = payload_.c_str();
		payload.decode(ptr, ptr + payload_.size());
		return payload;
	}

	void CqlProtocolColumnOption::setUdtPayload(const CqlProtocolColumnOptionUdtPayload& payload) {
		if (type_ != CqlColumnType::Udt) {
			throw CqlLogicException(CQL_CODEINFO, "type not matched");
		}
		payload_.resize(0);
		payload.encode(payload_);
	}

	CqlProtocolColumnOptionTuplePayload CqlProtocolColumnOption::getTuplePayload() const {
		if (type_ != CqlColumnType::Tuple) {
			throw CqlLogicException(CQL_CODEINFO, "type not matched");
		}
		CqlProtocolColumnOptionTuplePayload payload;
		const char* ptr = payload_.c_str();
		payload.decode(ptr, ptr + payload_.size());
		return payload;
	}

	void CqlProtocolColumnOption::setTuplePayload(const CqlProtocolColumnOptionTuplePayload& payload) {
		if (type_ != CqlColumnType::Tuple) {
			throw CqlLogicException(CQL_CODEINFO, "type not matched");
		}
		payload_.resize(0);
		payload.encode(payload_);
	}

	void CqlProtocolColumnOption::encode(seastar::sstring& data) const {
		if ((type_ == CqlColumnType::Custom ||
			type_ == CqlColumnType::List ||
			type_ == CqlColumnType::Map ||
			type_ == CqlColumnType::Set ||
			type_ == CqlColumnType::Udt ||
			type_ == CqlColumnType::Tuple) &&
			payload_.empty()) {
			throw CqlEncodeException(CQL_CODEINFO, "this column type require payload presented");
		}
		CqlProtocolShort type(static_cast<std::uint16_t>(type_));
		type.encode(data);
		data.append(payload_.c_str(), payload_.size());
	}

	void CqlProtocolColumnOption::decode(const char*& ptr, const char* end) {
		CqlProtocolShort type;
		type.decode(ptr, end);
		type_ = static_cast<CqlColumnType>(type.get());
		payload_.resize(0);
		if (type_ == CqlColumnType::Custom) {
			static thread_local CqlProtocolString payload;
			payload.decode(ptr, end);
			payload.encode(payload_);
		} else if (type_ == CqlColumnType::List) {
			static thread_local CqlProtocolColumnOptionListPayload payload;
			payload.decode(ptr, end);
			payload.encode(payload_);
		} else if (type_ == CqlColumnType::Map) {
			static thread_local CqlProtocolColumnOptionMapPayload payload;
			payload.decode(ptr, end);
			payload.encode(payload_);
		} else if (type_ == CqlColumnType::Set) {
			static thread_local CqlProtocolColumnOptionSetPayload payload;
			payload.decode(ptr, end);
			payload.encode(payload_);
		} else if (type_ == CqlColumnType::Udt) {
			static thread_local CqlProtocolColumnOptionUdtPayload payload;
			payload.decode(ptr, end);
			payload.encode(payload_);
		} else if (type_ == CqlColumnType::Tuple) {
			static thread_local CqlProtocolColumnOptionTuplePayload payload;
			payload.decode(ptr, end);
			payload.encode(payload_);
		}
	}

	void CqlProtocolColumnOptionCustomPayload::encode(seastar::sstring& data) const {
		value_.encode(data);
	}

	void CqlProtocolColumnOptionCustomPayload::decode(const char*& ptr, const char* end) {
		value_.decode(ptr, end);
	}

	void CqlProtocolColumnOptionListPayload::encode(seastar::sstring& data) const {
		elementType_.encode(data);
	}

	void CqlProtocolColumnOptionListPayload::decode(const char*& ptr, const char* end) {
		elementType_.decode(ptr, end);
	}

	void CqlProtocolColumnOptionMapPayload::encode(seastar::sstring& data) const {
		keyType_.encode(data);
		valueType_.encode(data);
	}

	void CqlProtocolColumnOptionMapPayload::decode(const char*& ptr, const char* end) {
		keyType_.decode(ptr, end);
		valueType_.decode(ptr, end);
	}

	void CqlProtocolColumnOptionSetPayload::encode(seastar::sstring& data) const {
		elementType_.encode(data);
	}

	void CqlProtocolColumnOptionSetPayload::decode(const char*& ptr, const char* end) {
		elementType_.decode(ptr, end);
	}

	void CqlProtocolColumnOptionUdtPayload::encode(seastar::sstring& data) const {
		CqlProtocolShort fieldsLength(fields_.size());
		if (fieldsLength.get() != fields_.size()) {
			throw CqlLogicException(CQL_CODEINFO, "too many udt fields cause overflow");
		}
		keySpace_.encode(data);
		udtName_.encode(data);
		fieldsLength.encode(data);
		for (const auto& pair : fields_) {
			pair.first.encode(data);
			pair.second.encode(data);
		}
	}

	void CqlProtocolColumnOptionUdtPayload::decode(const char*& ptr, const char* end) {
		CqlProtocolShort fieldsLength;
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

	void CqlProtocolColumnOptionTuplePayload::encode(seastar::sstring& data) const {
		CqlProtocolShort fieldsLength(types_.size());
		if (fieldsLength.get() != types_.size()) {
			throw CqlLogicException(CQL_CODEINFO, "too many tuple types cause overflow");
		}
		fieldsLength.encode(data);
		for (const auto& type : types_) {
			type.encode(data);
		}
	}

	void CqlProtocolColumnOptionTuplePayload::decode(const char*& ptr, const char* end) {
		CqlProtocolShort fieldsLength;
		fieldsLength.decode(ptr, end);
		types_.clear();
		for (std::size_t i = 0; i < fieldsLength.get(); ++i) {
			types_.emplace_back();
			types_.back().decode(ptr, end);
		}
	}
}

