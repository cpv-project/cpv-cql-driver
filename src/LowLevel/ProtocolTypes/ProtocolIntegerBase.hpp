#pragma once
#include <cstdint>
#include <cstring>
#include <seastar/core/byteorder.hh>
#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <CQLDriver/Common/CommonDefinitions.hpp>

namespace cql {
	/** Base class of integer types */
	template <class IntType>
	class ProtocolIntegerBase {
	public:
		/** Get the integer value */
		IntType get() const { return value_; }

		/** Set the integer value */
		void set(IntType value) { value_ = value; }

		/** Reset to initial state */
		void reset() { value_ = 0; }

		/** Encode to binary data */
		void encode(std::string& data) const {
			auto value = seastar::cpu_to_be(value_);
			data.append(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		/** Decode from binary data */
		void decode(const char*& ptr, const char* end) {
			if (CQL_UNLIKELY(ptr + sizeof(value_) > end)) {
				throw DecodeException(CQL_CODEINFO, "length not enough");
			}
			std::memcpy(&value_, ptr, sizeof(value_));
			value_ = seastar::be_to_cpu(value_);
			ptr += sizeof(value_);
		}

		/** Constructors */
		ProtocolIntegerBase() : value_(0) { }
		explicit ProtocolIntegerBase(IntType value) : value_(value) {}

	protected:
		IntType value_;
	};
}

