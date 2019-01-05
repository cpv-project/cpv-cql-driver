#include "./ProtocolInetAddr.hpp"
#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <CQLDriver/Common/Exceptions/EncodeException.hpp>
#include <CQLDriver/Common/CommonDefinitions.hpp>

namespace cql {
	/** Encode to binary data */
	void ProtocolInetAddr::encode(std::string& data) const {
		std::uint8_t size = static_cast<std::uint8_t>(value_.size());
		if (CQL_UNLIKELY(size == 0)) {
			throw EncodeException(CQL_CODEINFO, "address is undefined");
		}
		data.append(reinterpret_cast<const char*>(&size), sizeof(size));
		data.append(reinterpret_cast<const char*>(value_.data()), size);
	}

	/** Decode from binary data */
	void ProtocolInetAddr::decode(const char*& ptr, const char* end) {
		std::uint8_t size = 0;
		if (CQL_UNLIKELY(ptr + sizeof(size) > end)) {
			throw DecodeException(CQL_CODEINFO, "length not enough");
		}
		std::memcpy(&size, ptr, sizeof(size));
		ptr += sizeof(size);
		if (CQL_UNLIKELY(ptr + static_cast<std::size_t>(size) > end)) {
			throw DecodeException(CQL_CODEINFO, "length not enough");
		}
		if (size == sizeof(::in_addr)) {
			::in_addr addr_v4;
			std::memcpy(&addr_v4, ptr, size);
			value_ = seastar::net::inet_address(addr_v4);
			ptr += size;
		} else if (size == sizeof(::in6_addr)) {
			::in6_addr addr_v6;
			std::memcpy(&addr_v6, ptr, size);
			value_ = seastar::net::inet_address(addr_v6);
			ptr += size;
		} else {
			throw DecodeException(CQL_CODEINFO, "unsupport address size");
		}
	}
}

