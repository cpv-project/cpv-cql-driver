#include <cstdint>
#include <core/byteorder.hh>
#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include "CqlProtocolUuid.hpp"

namespace cql {
	/** Encode to binary data */
	void CqlProtocolUuid::encode(seastar::sstring& data) const {
		auto highBitsBe = seastar::cpu_to_be(value_.first);
		auto lowBitsBe = seastar::cpu_to_be(value_.second);
		data.append(reinterpret_cast<const char*>(&highBitsBe), sizeof(highBitsBe));
		data.append(reinterpret_cast<const char*>(&lowBitsBe), sizeof(lowBitsBe));
	}

	/** Decode from binary data */
	void CqlProtocolUuid::decode(const char*& ptr, const char* end) {
		static const constexpr std::size_t length = sizeof(value_.first) + sizeof(value_.second);
		if (ptr + length > end) {
			throw CqlDecodeException(CQL_CODEINFO, "length not enough");
		}
		std::memcpy(&value_.first, ptr, sizeof(value_.first));
		std::memcpy(&value_.second, ptr + sizeof(value_.first), sizeof(value_.second));
		value_.first = seastar::be_to_cpu(value_.first);
		value_.second = seastar::be_to_cpu(value_.second);
		ptr += length;
	}

	/** Constructors */
	CqlProtocolUuid::CqlProtocolUuid() : value_() { }
	CqlProtocolUuid::CqlProtocolUuid(const seastar::sstring& str) : value_() { set(str); }
	CqlProtocolUuid::CqlProtocolUuid(const UuidDataType& value) : value_(value) { }
}

