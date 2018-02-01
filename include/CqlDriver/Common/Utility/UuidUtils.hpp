#pragma once
#include <cstdint>
#include <utility>
#include <chrono>
#include <core/sstring.hh>

namespace cql {
	using UuidDataType = std::pair<std::uint64_t, std::uint64_t>;
	static const std::uint8_t EmptyUuidVersion = 0;
	static const std::uint8_t TimeUuidVersion = 1;
	static const std::uint8_t RandomUuidVersion = 4;

	/** Set the uuid by it's string representation */
	UuidDataType strToUuid(const seastar::sstring& str);

	/** Get the string representation of uuid */
	seastar::sstring uuidToStr(const UuidDataType& uuid);

	/** Make a empty uuid */
	UuidDataType makeEmptyUuid();

	/** Make a version 4 (random) uuid */
	UuidDataType makeRandomUuid();

	/** Make a version 1 (date-time and MAC address) uuid */
	UuidDataType makeTimeUuid();

	/** Make a minimum version 1 uuid of the specific time point, useful for search query */
	UuidDataType makeMinTimeUuid(const std::chrono::system_clock::time_point& timePoint);

	/** Make a maximum version 1 uuid of the specific time point, useful for search query */
	UuidDataType makeMaxTimeUuid(const std::chrono::system_clock::time_point& timePoint);

	/** Extrat version from uuid */
	std::uint8_t getVersionFromUuid(const UuidDataType& uuid);

	/** Extract time stamp from verion 1 uuid */
	std::chrono::system_clock::time_point getTimeFromUuid(const UuidDataType& uuid);
}

