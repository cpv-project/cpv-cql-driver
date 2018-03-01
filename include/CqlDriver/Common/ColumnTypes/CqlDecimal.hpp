#pragma once
#include "CqlColumnDefinitions.hpp"
#include "CqlDecimalOperations.hpp"

namespace cql {
	/**
	 * The decimal format represents an arbitrary-precision number.
	 * It contains an [int] "scale" component followed by a varint encoding
	 * of the unscaled value. The encoded value represents "<unscale>E<-scale>".
	 * In other words, "<unscaled> * 10 ^ (-1 * <scale>)"
	 */
	class CqlDecimal {
	public:
		/**
		 * (scale, unscaled)
		 * Example:
		 * (0, 123) => 123
		 * (1, 123) => 12.3
		 * (2, 123) => 1.23
		 */
		using CqlUnderlyingType = DecimalDataType;

		/** Get the decimal value */
		CqlUnderlyingType get() const& { return value_; }

		/** Get the mutable decimal vaule */
		CqlUnderlyingType& get() & { return value_; }

		/** Set the decimal value */
		void set(const CqlUnderlyingType& value) { value_ = value; }

		/** Set the decimal value from scale and unscaled integer */
		void set(CqlInt::CqlUnderlyingType scale, CqlVarInt::CqlUnderlyingType unscaled) {
			value_.first = scale;
			value_.second = unscaled;
		}

		/** Reset to initial state */
		void reset() { value_ = DecimalDataType(0, 0); }

		/** Set the decimal scale */
		void scale(CqlInt::CqlUnderlyingType scaleValue) {
			CqlInt::CqlUnderlyingType scaleDiff = value_.first - scaleValue;
			if (scaleDiff > 0) {
				value_.first = scaleValue;
				value_.second /= powInteger(10ULL, scaleDiff);
			} else if (scaleDiff < 0) {
				value_.first = scaleValue;
				value_.second *= powInteger(10ULL, -scaleDiff);
			}
		}

		/** Remove trailing zeros */
		void strip() {
			while (value_.first > 0 && value_.second.get() != 0 && value_.second.get() % 10 == 0) {
				--value_.first;
				value_.second /= 10;
			}
		}

		/** Encode to binary data */
		void encodeBody(seastar::sstring& data) const {
			value_.first.encodeBody(data);
			value_.second.encodeBody(data);
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, ColumnEncodeDecodeSizeType size) {
			if (size == 0) {
				reset(); // empty;
			} else if (size < CqlInt::CqlEncodeSize + 1) {
				throw CqlDecodeException(CQL_CODEINFO,
					"date length not enough, should be atleast", CqlInt::CqlEncodeSize + 1,
					"but actual is", size);
			} else {
				value_.first.decodeBody(ptr, CqlInt::CqlEncodeSize);
				value_.second.decodeBody(ptr + CqlInt::CqlEncodeSize, size - CqlInt::CqlEncodeSize);
			}
		}

		/** Constructors */
		CqlDecimal() : value_(0, 0) { }
		template <class... Args>
		CqlDecimal(Args&&... args) : value_() {
			set(std::forward<Args>(args)...);
		}

		/** Allow assign from whatever function "set" would take */
		template <class... Args>
		void operator=(Args&&... args) {
			set(std::forward<Args>(args)...);
		}

		/** Allow cast to decimal value implicitly */
		operator CqlUnderlyingType() const {
			return value_;
		}

	private:
		CqlUnderlyingType value_;
	};
}

