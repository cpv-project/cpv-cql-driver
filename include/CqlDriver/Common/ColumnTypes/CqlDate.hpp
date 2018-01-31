#pragma once
#include <iomanip>
#include "CqlDateTimeBase.hpp"

namespace cql {
	/**
	 * An unsigned integer representing days with epoch centered at 2^31.
	 * (unix epoch January 1st, 1970)
	 * A few examples:
	 * 0: -5077641-06-23
	 * 2^31: 1970-1-1
	 * 2^32: 5881580-07-11
	 */
	class CqlDate : private CqlDateTimeBase {
	public:
		using CqlDateTimeBase::CqlUnderlyingType;
		using CqlDateTimeBase::get;
		using CqlDateTimeBase::set;
		using CqlDateTimeBase::reset;
		using CqlDateTimeBase::CqlDateTimeBase;
		using CqlDateTimeBase::operator=;
		using CqlDateTimeBase::operator CqlUnderlyingType;
		using CqlDateTimeBase::operator std::time_t;
		using CqlDateTimeBase::operator std::tm;
		static const std::uint32_t EpochBoundary = 0x80000000;
		static const std::int32_t SecondsPerDay = 86400;

		/** Encode to binary data */
		void encode(seastar::sstring& data) const {
			// store local date in database, not utc date
			std::tm localTm = *this;
			std::time_t localTime = ::timegm(&localTm);
			std::uint32_t dbValue = EpochBoundary + static_cast<std::uint32_t>(localTime / SecondsPerDay);
			dbValue = seastar::cpu_to_be(dbValue);
			data.append(reinterpret_cast<const char*>(&dbValue), sizeof(dbValue));
		}

		/** Decode from binary data */
		void decode(const char* ptr, std::int32_t size) {
			std::uint32_t dbValue = 0;
			if (size == 0) {
				reset(); // empty
			} else if (size != sizeof(dbValue)) {
				throw CqlDecodeException(CQL_CODEINFO,
					"date length not matched, expected to be", sizeof(dbValue),
					"but actual is", size);
			} else {
				// load local date from database
				std::memcpy(&dbValue, ptr, sizeof(dbValue));
				dbValue = seastar::be_to_cpu(dbValue);
				std::time_t localTime = static_cast<std::time_t>(
					static_cast<std::int32_t>(dbValue - EpochBoundary) * SecondsPerDay);
				std::tm localTm = {};
				::gmtime_r(&localTime, &localTm);
				set(localTm);
			}
		}

		/** Create a CqlDate with specificed year-month-day */
		static CqlDate create(std::uint32_t year, std::uint8_t month, std::uint8_t day) {
			std::tm localTm = {};
			localTm.tm_mday = day; // [1, 31]
			localTm.tm_mon = month-1; // [0, 11]
			localTm.tm_year = year-1900; // years since 1900
			return CqlDate(localTm);
		}

		/** Create a CqlDate with today's date, in local timezone */
		static CqlDate today() {
			return CqlDate(std::chrono::system_clock::now());
		}
	};

	/** Write text description expressed in local timezone of date */
	std::ostream& operator<<(std::ostream& stream, const CqlDate& date) {
		std::tm localTm = date;
		stream << std::setfill('0') <<
			std::setw(4) << localTm.tm_year+1900 << "-" <<
			std::setw(2) << localTm.tm_mon+1 << "-" <<
			std::setw(2) << localTm.tm_mday;
		return stream;
	}
}

