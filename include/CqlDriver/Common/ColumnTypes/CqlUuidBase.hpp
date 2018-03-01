#pragma once
#include <cstdint>
#include <cstring>
#include <core/byteorder.hh>
#include "../Exceptions/CqlDecodeException.hpp"
#include "CqlUuidOperations.hpp"

namespace cql {
	/** Base class of uuid types */
	class CqlUuidBase {
	public:
		using CqlUnderlyingType = UuidDataType;
		static const std::size_t CqlEncodeSize = sizeof(UuidDataType::first) + sizeof(UuidDataType::second);

		/** Get the uuid value */
		UuidDataType get() const& { return value_; }

		/** Get the mutable uuid value */
		UuidDataType& get() & { return value_; }

		/** Set the uuid value */
		void set(const UuidDataType& value) { value_ = value; }

		/** Set the uuid value by it's string representation */
		void set(const seastar::sstring& str) { value_ = strToUuid(str); }

		/** Reset to initial state */
		void reset() { value_ = {}; }

		/** Check whether this uuid is empty */
		bool empty() const { return value_.first == 0 && value_.second == 0; }

		/** Get the string representation of this uuid */
		seastar::sstring str() const { return uuidToStr(value_); }

		/** Encode to binary data */
		void encodeBody(seastar::sstring& data) {
			auto highBitsBe = seastar::cpu_to_be(value_.first);
			auto lowBitsBe = seastar::cpu_to_be(value_.second);
			data.append(reinterpret_cast<const char*>(&highBitsBe), sizeof(highBitsBe));
			data.append(reinterpret_cast<const char*>(&lowBitsBe), sizeof(lowBitsBe));
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, std::int32_t size) {
			static const constexpr std::size_t length = sizeof(value_.first) + sizeof(value_.second);
			if (size == 0) {
				reset(); // empty
			} else if (size != length) {
				throw CqlDecodeException(CQL_CODEINFO,
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
		CqlUuidBase() : value_() { }
		template <class... Args>
		CqlUuidBase(Args&&... args) : value_() {
			set(std::forward<Args>(args)...);
		}

		/** Allow assign from whatever function "set" would take */
		template <class... Args>
		void operator=(Args&&... args) {
			set(std::forward<Args>(args)...);
		}

		/** Allow cast to uuid value implicitly */
		operator UuidDataType() const {
			return value_;
		}

	protected:
		UuidDataType value_;
	};
}

