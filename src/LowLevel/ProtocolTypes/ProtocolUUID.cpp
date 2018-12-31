#include <cstdint>
#include <cstring>
#include <seastar/core/byteorder.hh>
#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include "./ProtocolUUID.hpp"

namespace cql {
	/** Encode to binary data */
	void ProtocolUUID::encode(std::string& data) const {
		auto highBitsBe = seastar::cpu_to_be(value_.first);
		auto lowBitsBe = seastar::cpu_to_be(value_.second);
		data.append(reinterpret_cast<const char*>(&highBitsBe), sizeof(highBitsBe));
		data.append(reinterpret_cast<const char*>(&lowBitsBe), sizeof(lowBitsBe));
	}

	/** Decode from binary data */
	void ProtocolUUID::decode(const char*& ptr, const char* end) {
		static const constexpr std::size_t length = sizeof(value_.first) + sizeof(value_.second);
		if (ptr + length > end) {
			throw DecodeException(CQL_CODEINFO, "length not enough");
		}
		std::memcpy(&value_.first, ptr, sizeof(value_.first));
		std::memcpy(&value_.second, ptr + sizeof(value_.first), sizeof(value_.second));
		value_.first = seastar::be_to_cpu(value_.first);
		value_.second = seastar::be_to_cpu(value_.second);
		ptr += length;
	}

	/** Constructors */
	ProtocolUUID::ProtocolUUID() : value_() { }
	ProtocolUUID::ProtocolUUID(const std::string& str) : value_() { set(str); }
	ProtocolUUID::ProtocolUUID(const UUIDDataType& value) : value_(value) { }
}

