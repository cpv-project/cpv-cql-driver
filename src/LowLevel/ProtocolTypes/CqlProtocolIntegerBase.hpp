#pragma once
#include <cstdint>
#include <core/sstring.hh>
#include <core/byteorder.hh>
#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>

namespace cql {
	/** Base class of integer types */
	template <class IntType>
	class CqlProtocolIntegerBase {
	public:
		/** Get the integer value */
		IntType get() const { return value_; }

		/** Set the integer value */
		void set(IntType value) { value_ = value; }

		/** Reset to initial state */
		void reset() { value_ = 0; }

		/** Encode to binary data */
		void encode(seastar::sstring& data) const {
			auto value = seastar::cpu_to_be(value_);
			data.append(reinterpret_cast<const char*>(&value), sizeof(value));
		}

		/** Decode from binary data */
		void decode(const char*& ptr, const char* end) {
			if (ptr + sizeof(value_) > end) {
				throw CqlDecodeException(CQL_CODEINFO, "length not enough");
			}
			std::memcpy(&value_, ptr, sizeof(value_));
			value_ = seastar::be_to_cpu(value_);
			ptr += sizeof(value_);
		}

		/** Constructors */
		CqlProtocolIntegerBase() : value_(0) { }
		explicit CqlProtocolIntegerBase(IntType value) : value_(value) {}

	protected:
		IntType value_;
	};
}

