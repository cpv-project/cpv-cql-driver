#pragma once
#include "CqlIntegerBase.hpp"

namespace cql {
	/**
	 * A variable length two's complement encoding of a signed integer.
	 * examples:
	 *    0:   0x00
	 *    1:   0x01
	 *  127:   0x7f
	 *  128: 0x0080
	 *  129: 0x0081
	 *   -1:   0xff
	 * -128:   0x80
	 * -129: 0xff7f
	 * Note that positive numbers must use a most-significant byte with a value
	 * less than 0x80, because a most-significant bit of 1 indicates a negative
	 * value. Implementors should pad positive values that have a MSB >= 0x80
	 * with a leading 0x00 byte.
	 * Notice:
	 * This class can't store a variable integer that std::int64_t can't hold,
	 * I should provide a real varint later, but for now this is easier to implement
	 * and have better performance.
	 */
	class CqlVarInt : private CqlIntegerBase<std::int64_t> {
	public:
		using CqlIntegerBase::CqlUnderlyingType;
		using CqlIntegerBase::get;
		using CqlIntegerBase::set;
		using CqlIntegerBase::reset;
		using CqlIntegerBase::CqlIntegerBase;
		using CqlIntegerBase::operator=;
		using CqlIntegerBase::operator CqlUnderlyingType;
		using CqlIntegerBase::operator bool;

		/** Encode to binary data */
		void encode(seastar::sstring& data) const {
			auto value = seastar::cpu_to_be(value_);
			const std::uint8_t* valuePtr = reinterpret_cast<const uint8_t*>(&value);
			std::size_t valueSize = sizeof(value);
			while ((valueSize > 1) && (
				(valuePtr[0] == 0 && (valuePtr[1] & 0x80) == 0) ||
				(valuePtr[0] == 0xff && (valuePtr[1] & 0x80) == 0x80))) {
				++valuePtr;
				--valueSize;
			}
			data.append(reinterpret_cast<const char*>(valuePtr), valueSize);
		}

		/** Decode from binary data */
		void decode(const char* str, std::int32_t size) {
			if (size == 0) {
				value_ = 0; // empty
			} else if (static_cast<std::size_t>(size) > sizeof(value_)) {
				throw CqlDecodeException(CQL_CODEINFO,
					"varint is too large, it's not supported to decode, size is", size);
			} else {
				std::size_t paddingSize = sizeof(value_) - size;
				std::uint8_t paddingChar = (str[0] & 0x80) == 0 ? 0 : 0xff;
				std::uint8_t* valuePtr = reinterpret_cast<std::uint8_t*>(&value_);
				std::memset(valuePtr, paddingChar, paddingSize);
				std::memcpy(valuePtr+paddingSize, str, size);
				value_ = seastar::be_to_cpu(value_);
			}
		}
	};
}

