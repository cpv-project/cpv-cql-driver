#pragma once
#include "./ColumnDefinitions.hpp"
#include "./DecimalOperations.hpp"

namespace cql {
	/**
	 * The decimal format represents an arbitrary-precision number.
	 * It contains an [int] "scale" component followed by a varint encoding
	 * of the unscaled value. The encoded value represents "<unscale>E<-scale>".
	 * In other words, "<unscaled> * 10 ^ (-1 * <scale>)"
	 */
	class Decimal {
	public:
		/**
		 * (scale, unscaled)
		 * Example:
		 * (0, 123) => 123
		 * (1, 123) => 12.3
		 * (2, 123) => 1.23
		 */
		using UnderlyingType = DecimalDataType;

		/** Get the decimal value */
		UnderlyingType get() const& { return value_; }

		/** Get the mutable decimal vaule */
		UnderlyingType& get() & { return value_; }

		/** Set the decimal value */
		void set(const UnderlyingType& value) { value_ = value; }

		/** Set the decimal value from scale and unscaled integer */
		void set(Int::UnderlyingType scale, VarInt::UnderlyingType unscaled) {
			value_.first = scale;
			value_.second = unscaled;
		}

		/** Reset to initial state */
		void reset() { value_ = DecimalDataType(0, 0); }

		/** Set the decimal scale */
		void scale(Int::UnderlyingType scaleValue) {
			Int::UnderlyingType scaleDiff = value_.first - scaleValue;
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
			} else if (size < Int::EncodeSize + 1) {
				throw DecodeException(CQL_CODEINFO,
					"date length not enough, should be atleast", Int::EncodeSize + 1,
					"but actual is", size);
			} else {
				value_.first.decodeBody(ptr, Int::EncodeSize);
				value_.second.decodeBody(ptr + Int::EncodeSize, size - Int::EncodeSize);
			}
		}

		/** Constructors */
		Decimal() : value_(0, 0) { }
		template <class... Args>
		Decimal(Args&&... args) : value_() {
			set(std::forward<Args>(args)...);
		}

		/** Allow assign from whatever function "set" would take */
		template <class... Args>
		void operator=(Args&&... args) {
			set(std::forward<Args>(args)...);
		}

		/** Allow cast to decimal value implicitly */
		operator UnderlyingType() const {
			return value_;
		}

	private:
		UnderlyingType value_;
	};
}

