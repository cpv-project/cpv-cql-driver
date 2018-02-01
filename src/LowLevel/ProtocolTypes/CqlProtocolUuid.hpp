#pragma once
#include <CqlDriver/Common/Utility/UuidUtils.hpp>

namespace cql {
	/**
	 * A 16 bytes long uuid
	 * Example:
	 * CqlProtocolUuid uuid("00112233-4455-6677-8899-aabbccddeeff");
	 */
	class CqlProtocolUuid {
	public:
		/** Get the binary value of uuid */
		UuidDataType get() const { return value_; }

		/** Set the binary value of uuid */
		void set(UuidDataType value) { value_ = value; }

		/** Reset to initial state */
		void reset() { value_ = {}; }

		/** Get the string representation of uuid */
		seastar::sstring str() const { return uuidToStr(value_); }

		/** Set the uuid by it's string representation */
		void set(const seastar::sstring& str) { value_ = strToUuid(str); }

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructors */
		CqlProtocolUuid();
		explicit CqlProtocolUuid(const seastar::sstring& str);
		explicit CqlProtocolUuid(const UuidDataType& value);

	private:
		UuidDataType value_;
	};
}

