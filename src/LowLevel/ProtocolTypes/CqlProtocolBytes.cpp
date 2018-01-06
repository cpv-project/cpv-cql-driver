#include "CqlProtocolBytes.hpp"
#include <CqlDriver/Exceptions/CqlEncodeException.hpp>
#include <CqlDriver/Exceptions/CqlDecodeException.hpp>
#include <core/byteorder.hh>
#include <limits>

namespace cql {
	void CqlProtocolBytes::encode(seastar::sstring& data) const {
		if (isNull_) {
			// -1 = 0xffffffff
			data.append("\xff\xff\xff\xff", 4);
		} else {
			if (value_.size() > static_cast<std::size_t>(std::numeric_limits<LengthType>::max())) {
				throw CqlEncodeException(CQL_CODEINFO, "length too long");
			}
			LengthType size = seastar::cpu_to_be(static_cast<LengthType>(value_.size()));
			data.append(reinterpret_cast<const char*>(&size), sizeof(size));
			data.append(value_.c_str(), value_.size());
		}
	}

	void CqlProtocolBytes::decode(const char*& ptr, const char* end) {
		LengthType size = 0;
		if (ptr + sizeof(size) > end) {
			throw CqlDecodeException(CQL_CODEINFO, "length not enough");
		}
		std::memcpy(&size, ptr, sizeof(size));
		size = seastar::be_to_cpu(size);
		ptr += sizeof(size);
		if (size >= 0) {
			if (ptr + static_cast<std::size_t>(size) > end) {
				throw CqlDecodeException(CQL_CODEINFO, "length not enough");
			}
			isNull_ = false;
			value_.resize(0);
			value_.append(ptr, static_cast<std::size_t>(size));
			ptr += static_cast<std::size_t>(size);
		} else {
			isNull_ = true;
			value_.resize(0);
		}
	}
}

