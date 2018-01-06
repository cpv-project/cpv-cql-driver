#include "CqlProtocolInt.hpp"
#include <CqlDriver/Exceptions/CqlDecodeException.hpp>
#include <core/byteorder.hh>

namespace cql {
	void CqlProtocolInt::encode(seastar::sstring& data) const {
		auto value = seastar::cpu_to_be(value_);
		data.append(reinterpret_cast<const char*>(&value), sizeof(value));
	}

	void CqlProtocolInt::decode(const char*& ptr, const char* end) {
		if (ptr + sizeof(value_) > end) {
			throw CqlDecodeException(CQL_CODEINFO, "length not enough");
		}
		std::memcpy(&value_, ptr, sizeof(value_));
		value_ = seastar::be_to_cpu(value_);
		ptr += sizeof(value_);
	}
}

