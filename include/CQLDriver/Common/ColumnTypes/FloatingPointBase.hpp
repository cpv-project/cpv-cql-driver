#pragma once
#include <cstdint>
#include <cstring>
#include <cmath>
#include <limits>
#include "../Exceptions/DecodeException.hpp"
#include "./ColumnDefinitions.hpp"
#include "./FloatingPointOperations.hpp"

namespace cql {
	/** Base class of floating point types */
	template <class FloatType>
	class FloatingPointBase {
	public:
		using UnderlyingType = FloatType;
		static const std::size_t EncodeSize = sizeof(FloatType);

		static_assert(
			std::numeric_limits<FloatType>::is_iec559,
			"the compiler should use IEEE 754 floating point standard");

		/** Get the floating point value */
		FloatType get() const& { return value_; }

		/** Get the mutable floating point value */
		FloatType& get() & { return value_; }

		/** Set the floating point value */
		void set(FloatType value) { value_ = value; }

		/** Reset to initial state */
		void reset() { value_ = 0; }

		/**
		 * Compare floating point by test their difference with epsilon.
		 * It may not work with large numbers but works very well with smalls.
		 * The other way is abs(a-b) < max(abs(a), abs(b)) * epsilon, but that's not ideal too.
		 */
		bool approximatelyEquals(
			FloatType other,
			FloatType epsilon = std::numeric_limits<FloatType>::epsilon()) {
			if (std::isnan(value_) || std::isnan(other)) {
				return false;
			} else {
				return std::fabs(value_ - other) < epsilon;
			}
		}

		/** Encode to binary data */
		void encodeBody(std::string& data) {
			data.append(reinterpret_cast<const char*>(&value_), sizeof(value_));
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, const ColumnEncodeDecodeSizeType& size) {
			if (size == 0) {
				value_ = 0; // empty
			} else if (CQL_UNLIKELY(size != sizeof(value_))) {
				throw DecodeException(CQL_CODEINFO,
					"floating point length not matched, expected to be", sizeof(value_),
					"but actual is", size);
			} else {
				std::memcpy(&value_, ptr, sizeof(value_));
			}
		}

		/** Constructor */
		FloatingPointBase() : value_(0) { }
		// cppcheck-suppress noExplicitConstructor
		FloatingPointBase(FloatType value) : value_(value) { }

		/** Allow assign from floating point */
		void operator=(FloatType value) {
			value_ = value;
		}

		/** Allow cast to floating point implicitly */
		operator FloatType() const {
			return value_;
		}

	protected:
		FloatType value_;
	};
}

