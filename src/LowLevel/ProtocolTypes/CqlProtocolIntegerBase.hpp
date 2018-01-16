#pragma once
#include <cstdint>
#include <core/sstring.hh>
#include <core/byteorder.hh>
#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>

namespace cql {
	/**
	 * Base class of integer types
	 */
	template <class IntType>
	class CqlProtocolIntegerBase {
	public:
		IntType get() const { return value_; }
		void set(IntType value) { value_ = value; }
		void reset() { value_ = 0; }

		void encode(seastar::sstring& data) const {
			auto value = seastar::cpu_to_be(value_);
			data.append(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		void decode(const char*& ptr, const char* end) {
			if (ptr + sizeof(value_) > end) {
				throw CqlDecodeException(CQL_CODEINFO, "length not enough");
			}
			std::memcpy(&value_, ptr, sizeof(value_));
			value_ = seastar::be_to_cpu(value_);
			ptr += sizeof(value_);
		}

		CqlProtocolIntegerBase() : value_(0) { }
		explicit CqlProtocolIntegerBase(IntType value) : value_(value) {}

	protected:
		IntType value_;
	};
}

