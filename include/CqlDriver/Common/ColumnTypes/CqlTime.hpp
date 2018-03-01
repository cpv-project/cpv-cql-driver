#pragma once
#include <iomanip>
#include "CqlColumnDefinitions.hpp"
#include "CqlDateTimeBase.hpp"

namespace cql {
	/**
	 * An 8 byte two's complement long representing nanoseconds since midnignt.
	 * Valid values are in the range 0 to 86'3999'9999'9999
	 */
	class CqlTime : private CqlDateTimeBase {
	public:
		using CqlDateTimeBase::CqlUnderlyingType;
		using CqlDateTimeBase::get;
		using CqlDateTimeBase::set;
		using CqlDateTimeBase::reset;
		using CqlDateTimeBase::operator CqlUnderlyingType;
		using CqlDateTimeBase::operator std::time_t;
		using CqlDateTimeBase::operator std::tm;
		static const std::size_t CqlEncodeSize = sizeof(std::uint64_t);
		static const std::uint32_t SecondsPerDay = 86400;
		static const std::uint32_t NanoSecondsPerSecond = 1'000'000'000;

		/** Set duration since midnight of this day, in local timezone */
		template <class Rep, class Period>
		void set(std::chrono::duration<Rep, Period> duration) {
			std::chrono::nanoseconds nanoSeconds(duration);
			std::time_t localTime = nanoSeconds.count() / NanoSecondsPerSecond;
			std::tm localTm = {};
			::gmtime_r(&localTime, &localTm);
			set(localTm);
			value_ += std::chrono::nanoseconds(nanoSeconds.count() % NanoSecondsPerSecond);
		}

		/** Reset to initial state */
		void reset() {
			set(std::chrono::nanoseconds(0));
		}

		/** Encode to binary data */
		void encodeBody(seastar::sstring& data) const {
			// store local time in database, not utc time
			std::chrono::nanoseconds nanoSeconds = *this;
			std::uint64_t dbValue = nanoSeconds.count();
			dbValue = seastar::cpu_to_be(dbValue);
			data.append(reinterpret_cast<const char*>(&dbValue), sizeof(dbValue));
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, ColumnEncodeDecodeSizeType size) {
			std::uint64_t dbValue = 0;
			if (size == 0) {
				reset(); // empty
			} else if (size != sizeof(dbValue)) {
				throw CqlDecodeException(CQL_CODEINFO,
					"time length not matched, expected to be", sizeof(dbValue),
					"but actual is", size);
			} else {
				// load local time from database
				std::memcpy(&dbValue, ptr, sizeof(dbValue));
				dbValue = seastar::be_to_cpu(dbValue);
				set(std::chrono::nanoseconds(dbValue));
			}
		}

		/** Constructors */
		CqlTime() : CqlDateTimeBase() { reset(); }
		template <class... Args>
		CqlTime(Args&&... args) : CqlDateTimeBase() {
			set(std::forward<Args>(args)...);
		}

		/** Allow assign from whatever function "set" would take */
		template <class... Args>
		void operator=(Args&&... args) {
			set(std::forward<Args>(args)...);
		}

		/** Get total nanoseconds since midnight of this day, in local timezone */
		operator std::chrono::nanoseconds() const {
			std::tm localTm = *this;
			std::time_t localTime = ::timegm(&localTm);
			std::uint64_t nanoseconds = (localTime % SecondsPerDay) * NanoSecondsPerSecond;
			nanoseconds += (
				std::chrono::nanoseconds(value_.time_since_epoch()).count() % NanoSecondsPerSecond);
			return std::chrono::nanoseconds(nanoseconds);
		}

		/** Get total microseconds since midnight of this day, in local timezone */
		operator std::chrono::microseconds() const {
			return std::chrono::duration_cast<std::chrono::microseconds>(
				static_cast<std::chrono::nanoseconds>(*this));
		}

		/** Get total milliseconds since midnight of this day, in local timezone */
		operator std::chrono::milliseconds() const {
			return std::chrono::duration_cast<std::chrono::milliseconds>(
				static_cast<std::chrono::nanoseconds>(*this));
		}

		/** Get total seconds since midnight of this day, in local timezone */
		operator std::chrono::seconds() const {
			return std::chrono::duration_cast<std::chrono::seconds>(
				static_cast<std::chrono::nanoseconds>(*this));
		}

		/** Get total minutes since midnight of this day, in local timezone */
		operator std::chrono::minutes() const {
			return std::chrono::duration_cast<std::chrono::minutes>(
				static_cast<std::chrono::nanoseconds>(*this));
		}

		/** Get total hours since midnight of this day, in local timezone */
		operator std::chrono::hours() const {
			return std::chrono::duration_cast<std::chrono::hours>(
				static_cast<std::chrono::nanoseconds>(*this));
		}

		/** Create a CqlTime with specificed hour:minute:second */
		static CqlTime create(std::uint8_t hour, std::uint8_t minute, std::uint8_t second) {
			std::tm localTm = {};
			localTm.tm_sec = second; // [0, 60]
			localTm.tm_min = minute; // [0, 59]
			localTm.tm_hour = hour; // [0, 23]
			localTm.tm_mday = 1; // [1, 31]
			localTm.tm_mon = 1; // [0, 11]
			localTm.tm_year = 70; // years since 1900
			return CqlTime(localTm);
		}

		/** Create a CqlTime with now, in local timezone */
		static CqlTime now() {
			return CqlTime(std::chrono::system_clock::now());
		}
	};

	/** Write text description expressed in local timezone of time */
	std::ostream& operator<<(std::ostream& stream, const CqlTime& time) {
		std::tm localTm = time;
		std::chrono::nanoseconds nanoSeconds = time;
		stream << std::setfill('0') <<
			std::setw(2) << localTm.tm_hour << ":" <<
			std::setw(2) << localTm.tm_min << ":" <<
			std::setw(2) << localTm.tm_sec << "." <<
			std::setw(9) << (nanoSeconds.count() % CqlTime::NanoSecondsPerSecond);
		return stream;
	}
}

