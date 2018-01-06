#include "CqlProtocolUuid.hpp"
#include <CqlDriver/Exceptions/CqlInternalException.hpp>
#include <CqlDriver/Exceptions/CqlFormatException.hpp>
#include <CqlDriver/Utility/StringUtils.hpp>
#include <core/byteorder.hh>
#include <random>

namespace cql {
	std::pair<std::uint64_t, std::uint64_t> CqlProtocolUuid::get() const {
		return std::make_pair(highBits_, lowBits_);
	}

	void CqlProtocolUuid::set(std::pair<std::uint64_t, std::uint64_t> value) {
		highBits_ = value.first;
		lowBits_ = value.second;
	}

	seastar::sstring CqlProtocolUuid::str() const {
		// example: 00112233-4455-6677-8899-aabbccddeeff
		seastar::sstring result;
		dumpIntToHex(static_cast<std::uint32_t>(highBits_ >> 32), result);
		result.append("-", 1);
		dumpIntToHex(static_cast<std::uint16_t>((highBits_ >> 16) & 0xffff), result);
		result.append("-", 1);
		dumpIntToHex(static_cast<std::uint16_t>(highBits_ & 0xffff), result);
		result.append("-", 1);
		dumpIntToHex(static_cast<std::uint16_t>((lowBits_ >> 48) & 0xffff), result);
		result.append("-", 1);
		dumpIntToHex(static_cast<std::uint16_t>((lowBits_ >> 32) & 0xffff), result);
		dumpIntToHex(static_cast<std::uint32_t>(lowBits_ & 0xffffffff), result);
		return result;
	}

	void CqlProtocolUuid::set(const seastar::sstring& str) {
		// example: 00112233-4455-6677-8899-aabbccddeeff
		if (str.size() != 36) {
			throw cql::CqlFormatException(CQL_CODEINFO, "invalid uuid string: size should be 36");
		}
		std::uint32_t a = 0;
		std::uint16_t b = 0;
		std::uint16_t c = 0;
		std::uint16_t d = 0;
		std::uint16_t e = 0;
		std::uint32_t f = 0;
		if (!loadIntFromHex(str.c_str(), a) ||
			!loadIntFromHex(str.c_str() + 9, b) ||
			!loadIntFromHex(str.c_str() + 14, c) ||
			!loadIntFromHex(str.c_str() + 19, d) ||
			!loadIntFromHex(str.c_str() + 24, e) ||
			!loadIntFromHex(str.c_str() + 28, f)) {
			throw cql::CqlFormatException(CQL_CODEINFO, "invalid uuid string: contains non-hex character");
		}
		highBits_ = (static_cast<std::uint64_t>(a) << 32) | (static_cast<std::uint64_t>(b) << 16) | c;
		lowBits_ = (static_cast<std::uint64_t>(d) << 48) | (static_cast<std::uint64_t>(e) << 32) | f;
	}

	void CqlProtocolUuid::encode(seastar::sstring& data) const {
		auto highBitsBe = seastar::cpu_to_be(highBits_);
		auto lowBitsBe = seastar::cpu_to_be(lowBits_);
		data.append(reinterpret_cast<const char*>(&highBitsBe), sizeof(highBitsBe));
		data.append(reinterpret_cast<const char*>(&lowBitsBe), sizeof(lowBitsBe));
	}

	void CqlProtocolUuid::decode(const char*& ptr, const char* end) {
		static const constexpr std::size_t length = sizeof(highBits_) + sizeof(lowBits_);
		if (ptr + length > end) {
			throw CqlInternalException(CQL_CODEINFO, "decode failed: length not enough");
		}
		std::memcpy(&highBits_, ptr, sizeof(highBits_));
		std::memcpy(&lowBits_, ptr + sizeof(highBits_), sizeof(lowBits_));
		highBits_ = seastar::be_to_cpu(highBits_);
		lowBits_ = seastar::be_to_cpu(lowBits_);
		ptr += length;
	}

	const CqlProtocolUuid& CqlProtocolUuid::getEmpty() {
		static CqlProtocolUuid staticEmpty(0, 0);
		return staticEmpty;
	}

	CqlProtocolUuid CqlProtocolUuid::makeRandom() {
		static thread_local std::random_device urandom;
		static thread_local std::uniform_int_distribution<std::uint8_t> dist(0, 255);
		CqlProtocolUuid uuid(0, 0);
		std::uint8_t* highBitsPtr = reinterpret_cast<std::uint8_t*>(&uuid.highBits_);
		std::uint8_t* lowBitsPtr = reinterpret_cast<std::uint8_t*>(&uuid.lowBits_);
		for (std::size_t i = 0; i < sizeof(highBits_); ++i) {
			highBitsPtr[i] = dist(urandom);
		}
		for (std::size_t i = 0; i < sizeof(lowBits_); ++i) {
			lowBitsPtr[i] = dist(urandom);
		}
		uuid.highBits_ = (uuid.highBits_ & 0xffff'ffff'ffff'0fff) | 0x4000; // version 4
		uuid.lowBits_ = (uuid.lowBits_ & 0x3fff'ffff'ffff'ffff) | 0x8000'0000'0000'0000; // variant 1
		return uuid;
	}

	CqlProtocolUuid::CqlProtocolUuid(const seastar::sstring& value) :
		highBits_(), lowBits_() { set(value); }

	CqlProtocolUuid::CqlProtocolUuid(std::pair<std::uint64_t, std::uint64_t> value) :
		highBits_(value.first), lowBits_(value.second) { }

	CqlProtocolUuid::CqlProtocolUuid(std::uint64_t highBits, std::uint64_t lowBits) :
		highBits_(highBits), lowBits_(lowBits) { }
}

