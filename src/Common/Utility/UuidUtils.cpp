#include <random>
#include <core/byteorder.hh>
#include <CqlDriver/Common/Exceptions/CqlFormatException.hpp>
#include <CqlDriver/Common/Exceptions/CqlUuidConflictException.hpp>
#include <CqlDriver/Common/Utility/StringUtils.hpp>
#include <CqlDriver/Common/Utility/UuidUtils.hpp>

namespace cql {
	namespace {
		// NOTICE: random_device may not work on windows,
		// but this library also does not work on windows
		thread_local std::random_device URandom;
		static_assert(sizeof(URandom()) >= sizeof(std::uint32_t),
			"random_device should return a atleast 32 bit integer");
		static const std::uint32_t MaxClockId = 0b11'1111'1111'1111; // 16383
		static const std::uint64_t EpochSinceUuid1TimeStampBase = 122'192'928'000'000'000; // 1582-10-15

		/** Convert a time point to time stamp of uuid 1 */
		std::uint64_t toTimeUuidTimeStamp(const std::chrono::system_clock::time_point& timePoint) {
			// the number of 100 nanoseconds intervals since 1582-10-15
			std::chrono::system_clock::duration sinceEpoch = timePoint.time_since_epoch();
			std::uint64_t timeStamp = (
				(std::chrono::duration_cast<
					std::chrono::milliseconds>(sinceEpoch).count() * 10000ULL) +
				((std::chrono::duration_cast<
					std::chrono::nanoseconds>(sinceEpoch).count() / 100ULL) % 10000) +
				EpochSinceUuid1TimeStampBase);
			return timeStamp;
		}

		/** Convert a time stamp of uuid 1 to time point */
		std::chrono::system_clock::time_point fromTimeUuidTimeStamp(std::uint64_t timeStamp) {
			auto result = std::chrono::system_clock::from_time_t(0);
			result += std::chrono::milliseconds(
				(timeStamp - EpochSinceUuid1TimeStampBase) / 10000);
			result += std::chrono::nanoseconds((timeStamp % 10000) * 100);
			return result;
		}
	}

	/** Set the uuid by it's string representation */
	UuidDataType strToUuid(const seastar::sstring& str) {
		// example: 00112233-4455-6677-8899-aabbccddeeff
		if (str.size() != 36) {
			throw cql::CqlFormatException(CQL_CODEINFO,
				"invalid uuid string: size should be 36, str is", str);
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
			throw cql::CqlFormatException(CQL_CODEINFO,
				"invalid uuid string: contains non-hex character, str is", str);
		}
		std::uint64_t highBits = (
			static_cast<std::uint64_t>(a) << 32) | (static_cast<std::uint64_t>(b) << 16) | c;
		std::uint64_t lowBits = (
			static_cast<std::uint64_t>(d) << 48) | (static_cast<std::uint64_t>(e) << 32) | f;
		return UuidDataType(highBits, lowBits);
	}

	/** Get the string representation of uuid */
	seastar::sstring uuidToStr(const UuidDataType& uuid) {
		// example: 00112233-4455-6677-8899-aabbccddeeff
		seastar::sstring result;
		dumpIntToHex(static_cast<std::uint32_t>(uuid.first >> 32), result);
		result.append("-", 1);
		dumpIntToHex(static_cast<std::uint16_t>((uuid.first >> 16) & 0xffff), result);
		result.append("-", 1);
		dumpIntToHex(static_cast<std::uint16_t>(uuid.first & 0xffff), result);
		result.append("-", 1);
		dumpIntToHex(static_cast<std::uint16_t>((uuid.second >> 48) & 0xffff), result);
		result.append("-", 1);
		dumpIntToHex(static_cast<std::uint16_t>((uuid.second >> 32) & 0xffff), result);
		dumpIntToHex(static_cast<std::uint32_t>(uuid.second & 0xffffffff), result);
		return result;
	}

	/** Make a empty uuid */
	UuidDataType makeEmptyUuid() {
		return UuidDataType(0, 0);
	}

	/** Make a version 4 (random) uuid */
	UuidDataType makeRandomUuid() {
		UuidDataType uuid;
		uuid.first = (static_cast<std::uint64_t>(URandom()) << 32) | URandom();
		uuid.second = (static_cast<std::uint64_t>(URandom()) << 32) | URandom();
		uuid.first = (uuid.first & 0xffff'ffff'ffff'0fff) | 0x4000; // version 4
		uuid.second = (uuid.second & 0x3fff'ffff'ffff'ffff) | 0x8000'0000'0000'0000; // variant 1
		return uuid;
	}

	/** Make a version 1 (date-time and MAC address) uuid */
	UuidDataType makeTimeUuid(
		std::uint64_t timeStamp, std::uint32_t clockId, std::uint64_t nodeId) {
		// check: https://www.famkruithof.net/guid-uuid-timebased.html
		// example: 58e0a7d7-eebc-11d8-9669-0800200c9a66
		// - timestamp: 1db'eebc'58e0a7d7
		// - clock id: 1669
		// - node id: 0800200c9a66
		UuidDataType uuid;
		uuid.first = (
			((timeStamp & 0x0000'0000'ffff'ffffULL) << 32) |
			((timeStamp & 0x0000'ffff'0000'0000ULL) >> 16) |
			0x0000'0000'0000'1000ULL | // version 1
			((timeStamp & 0x0fff'0000'0000'0000ULL) >> 48));
		uuid.second = (
			0x8000'0000'0000'0000ULL | //variant 1
			((clockId & 0x3fffULL) << 48) |
			(nodeId & 0x0000'ffff'ffff'ffffULL));
		return uuid;
	}

	/** Make a version 1 (date-time and MAC address) uuid */
	UuidDataType makeTimeUuid() {
		thread_local static std::uint64_t staticNodeId = 0;
		thread_local static std::uint64_t staticLastTimeStamp = 0;
		thread_local static std::uint32_t staticClockId = 0;
		if (staticNodeId == 0) {
			// use random value as mac address because each core won't share clock id
			staticNodeId = (static_cast<std::uint64_t>(URandom()) << 32) | URandom();
		}
		std::uint64_t timeStamp = toTimeUuidTimeStamp(std::chrono::system_clock::now());
		if (timeStamp != staticLastTimeStamp) {
			staticLastTimeStamp = timeStamp;
			staticClockId = 0;
		} else if (staticClockId > MaxClockId) {
			throw CqlUuidConflictException(CQL_CODEINFO,
				"generate time uuid failed, clock id is exhausted in this 100 nanoseconds");
		}
		std::uint32_t clockId = staticClockId++;
		return makeTimeUuid(timeStamp, clockId, staticNodeId);
	}

	/** Make a minimum version 1 uuid of the specific time point, useful for search query */
	UuidDataType makeMinTimeUuid(const std::chrono::system_clock::time_point& timePoint) {
		std::uint64_t timeStamp = toTimeUuidTimeStamp(timePoint);
		return makeTimeUuid(timeStamp, 0, 0);
	}

	/** Make a maximum version 1 uuid of the specific time point, useful for search query */
	UuidDataType makeMaxTimeUuid(const std::chrono::system_clock::time_point& timePoint) {
		std::uint64_t timeStamp = toTimeUuidTimeStamp(timePoint);
		return makeTimeUuid(timeStamp, MaxClockId, 0xffff'ffff'ffff'ffffULL);
	}

	/** Extrat version from uuid */
	std::uint8_t getVersionFromUuid(const UuidDataType& uuid) {
		return (uuid.first & 0x0000'0000'0000'f000ULL) >> 12;
	}

	/** Extract time stamp from verion 1 uuid */
	std::chrono::system_clock::time_point getTimeFromUuid(const UuidDataType& uuid) {
		if (getVersionFromUuid(uuid) != TimeUuidVersion) {
			// version 2 is unsupported yet
			throw CqlFormatException(CQL_CODEINFO,
				"get time from uuid failed: version should be 1, uuid is", uuidToStr(uuid));
		}
		std::uint64_t timeStamp = (
			((uuid.first & 0xffff'ffff'0000'0000ULL) >> 32) |
			((uuid.first & 0x0000'0000'ffff'0000ULL) << 16) |
			((uuid.first & 0x0000'0000'0000'0fffULL) << 48));
		return fromTimeUuidTimeStamp(timeStamp);
	}
}

