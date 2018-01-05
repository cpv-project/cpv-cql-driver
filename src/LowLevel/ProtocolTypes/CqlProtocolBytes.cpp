#include "CqlProtocolBytes.hpp"
#include <CqlDriver/Exceptions/CqlInternalException.hpp>
#include <core/byteorder.hh>

namespace cql {
	void CqlProtocolBytes::encode(seastar::sstring& data) const {
		if (isNull_) {
			data.append("\xff\xff\xff\xff", 4);
		} else {
			std::int32_t size = seastar::cpu_to_be(static_cast<std::int32_t>(value_.size()));
			data.append(reinterpret_cast<const char*>(&size), sizeof(size));
			data.append(value_.c_str(), value_.size());
		}
	}

	void CqlProtocolBytes::decode(const char*& ptr, const char* end) {
		std::int32_t size = 0;
		if (ptr + sizeof(size) > end) {
			throw CqlInternalException(CQL_CODEINFO, "decode failed: length not enough");
		}
		std::memcpy(&size, ptr, sizeof(size));
		size = seastar::be_to_cpu(size);
		ptr += sizeof(size);
		if (size >= 0) {
			if (ptr + static_cast<std::size_t>(size) > end) {
				throw CqlInternalException(CQL_CODEINFO, "decode failed: length not enough");
			}
			isNull_ = false;
			value_.append(ptr, static_cast<std::size_t>(size));
			ptr += static_cast<std::size_t>(size);
		} else {
			isNull_ = true;
			value_.resize(0);
		}
	}
}

