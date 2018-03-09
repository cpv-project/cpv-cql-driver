#pragma once
#include <CQLDriver/Common/Utility/UUIDUtils.hpp>

namespace cql {
	/**
	 * A 16 bytes long uuid
	 * Example:
	 * ProtocolUUID uuid("00112233-4455-6677-8899-aabbccddeeff");
	 */
	class ProtocolUUID {
	public:
		/** Get the binary value of uuid */
		UUIDDataType get() const { return value_; }

		/** Set the binary value of uuid */
		void set(UUIDDataType value) { value_ = value; }

		/** Reset to initial state */
		void reset() { value_ = {}; }

		/** Get the string representation of uuid */
		seastar::sstring str() const { return uuidToStr(value_); }

		/** Set the uuid by it's string representation */
		void set(const seastar::sstring& str) { value_ = strToUUID(str); }

		/** Encode and decode functions */
		void encode(seastar::sstring& data) const;
		void decode(const char*& ptr, const char* end);

		/** Constructors */
		ProtocolUUID();
		explicit ProtocolUUID(const seastar::sstring& str);
		explicit ProtocolUUID(const UUIDDataType& value);

	private:
		UUIDDataType value_;
	};
}

