#pragma once
#include <cstdint>
#include <cstring>
#include <core/byteorder.hh>
#include "../Exceptions/DecodeException.hpp"
#include "./ColumnDefinitions.hpp"
#include "./UUIDOperations.hpp"

namespace cql {
	/** Base class of uuid types */
	class UUIDBase {
	public:
		using UnderlyingType = UUIDDataType;
		static const std::size_t EncodeSize = sizeof(UUIDDataType::first) + sizeof(UUIDDataType::second);

		/** Get the uuid value */
		UUIDDataType get() const& { return value_; }

		/** Get the mutable uuid value */
		UUIDDataType& get() & { return value_; }

		/** Set the uuid value */
		void set(const UUIDDataType& value) { value_ = value; }

		/** Set the uuid value by it's string representation */
		void set(const std::string& str) { value_ = strToUUID(str); }

		/** Reset to initial state */
		void reset() { value_ = {}; }

		/** Check whether this uuid is empty */
		bool empty() const { return value_.first == 0 && value_.second == 0; }

		/** Get the string representation of this uuid */
		std::string str() const { return uuidToStr(value_); }

		/** Encode to binary data */
		void encodeBody(std::string& data) {
			auto highBitsBe = seastar::cpu_to_be(value_.first);
			auto lowBitsBe = seastar::cpu_to_be(value_.second);
			data.append(reinterpret_cast<const char*>(&highBitsBe), sizeof(highBitsBe));
			data.append(reinterpret_cast<const char*>(&lowBitsBe), sizeof(lowBitsBe));
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, const ColumnEncodeDecodeSizeType& size) {
			static const constexpr std::size_t length = sizeof(value_.first) + sizeof(value_.second);
			if (size == 0) {
				reset(); // empty
			} else if (size != length) {
				throw DecodeException(CQL_CODEINFO,
					"uuid length not matched, expected to be", length,
					"but actual is", size);
			} else {
				std::memcpy(&value_.first, ptr, sizeof(value_.first));
				std::memcpy(&value_.second, ptr + sizeof(value_.first), sizeof(value_.second));
				value_.first = seastar::be_to_cpu(value_.first);
				value_.second = seastar::be_to_cpu(value_.second);
			}
		}

		/** Constructors */
		UUIDBase() : value_() { }
		template <class... Args>
		explicit UUIDBase(Args&&... args) : value_() {
			set(std::forward<Args>(args)...);
		}

		/** Allow assign from whatever function "set" would take */
		template <class... Args>
		void operator=(Args&&... args) {
			set(std::forward<Args>(args)...);
		}

		/** Allow cast to uuid value implicitly */
		operator UUIDDataType() const {
			return value_;
		}

	protected:
		UUIDDataType value_;
	};
}

