#pragma once
#include <iomanip>
#include "./ColumnDefinitions.hpp"
#include "./DateTimeBase.hpp"

namespace cql {
	/**
	 * An 8 byte two's complement integer representing a millisecond-precision
	 * offset from the unix epoch (00:00:00, January 1st, 1970).
	 * Negative values represent a negative offset from the epoch.
	 */
	class Timestamp : private DateTimeBase {
	public:
		using DateTimeBase::UnderlyingType;
		using DateTimeBase::get;
		using DateTimeBase::set;
		using DateTimeBase::reset;
		using DateTimeBase::operator UnderlyingType;
		using DateTimeBase::operator std::time_t;
		using DateTimeBase::operator std::tm;
		static const std::size_t EncodeSize = sizeof(std::int64_t);
		static const std::uint32_t MilliSecondsPerSecond = 1'000;

		/** Set duration since unix epoch, in utc */
		template <class Rep, class Period>
		void set(std::chrono::duration<Rep, Period> duration) {
			value_ = std::chrono::system_clock::from_time_t(0) + duration;
		}

		/** Encode to binary data */
		void encodeBody(seastar::sstring& data) const {
			// store utc timestamp in database
			std::chrono::milliseconds milliseconds = *this;
			std::int64_t dbValue = milliseconds.count();
			dbValue = seastar::cpu_to_be(dbValue);
			data.append(reinterpret_cast<const char*>(&dbValue), sizeof(dbValue));
		}

		/** Decode from binary data */
		void decodeBody(const char* ptr, ColumnEncodeDecodeSizeType size) {
			std::int64_t dbValue = 0;
			if (size == 0) {
				reset(); // empty
			} else if (size != sizeof(dbValue)) {
				throw DecodeException(CQL_CODEINFO,
					"timestamp length not matched, expected to be", sizeof(dbValue),
					"but actual is", size);
			} else {
				// load utc timestamp from database
				// in c++ specification milliseconds is required to cover a range of atleast 292 years
				// but in most implementation milliseconds uses int64_t
				std::memcpy(&dbValue, ptr, sizeof(dbValue));
				dbValue = seastar::be_to_cpu(dbValue);
				set(std::chrono::milliseconds(dbValue));
			}
		}

		/** Constructors */
		Timestamp() : DateTimeBase() { }
		template <class... Args>
		Timestamp(Args&&... args) : DateTimeBase() {
			set(std::forward<Args>(args)...);
		}

		/** Allow assign from whatever function "set" would take */
		template <class... Args>
		void operator=(Args&&... args) {
			set(std::forward<Args>(args)...);
		}

		/** Get total nanoseconds since unix epoch, in utc */
		operator std::chrono::nanoseconds() const {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(
				value_.time_since_epoch());
		}

		/** Get total microseconds since unix epoch, in utc */
		operator std::chrono::microseconds() const {
			return std::chrono::duration_cast<std::chrono::microseconds>(
				value_.time_since_epoch());
		}

		/** Get total milliseconds since unix epoch, in utc */
		operator std::chrono::milliseconds() const {
			return std::chrono::duration_cast<std::chrono::milliseconds>(
				value_.time_since_epoch());
		}

		/** Get total seconds since unix epoch, in utc */
		operator std::chrono::seconds() const {
			return std::chrono::duration_cast<std::chrono::seconds>(
				value_.time_since_epoch());
		}

		/** Get total minutes since unix epoch, in utc */
		operator std::chrono::minutes() const {
			return std::chrono::duration_cast<std::chrono::minutes>(
				value_.time_since_epoch());
		}

		/** Get total hours since unix epoch, in utc */
		operator std::chrono::hours() const {
			return std::chrono::duration_cast<std::chrono::hours>(
				value_.time_since_epoch());
		}

		/** Create a Timestamp with specificed year-month-day hour:minute:second */
		static Timestamp create(
			std::uint32_t year, std::uint8_t month, std::uint8_t day,
			std::uint8_t hour, std::uint8_t minute, std::uint8_t second) {
			std::tm localTm = {};
			localTm.tm_sec = second; // [0, 60]
			localTm.tm_min = minute; // [0, 59]
			localTm.tm_hour = hour; // [0, 23]
			localTm.tm_mday = day; // [1, 31]
			localTm.tm_mon = month-1; // [0, 11]
			localTm.tm_year = year-1900; // years since 1900
			return Timestamp(localTm);
		}

		/** Create a Time with now, in local timezone */
		static Timestamp now() {
			return Timestamp(std::chrono::system_clock::now());
		}
	};

	/** Write text description expressed in local timezone of timestamp */
	std::ostream& operator<<(std::ostream& stream, const Timestamp& time) {
		std::tm localTm = time;
		std::chrono::milliseconds milliSeconds = time;
		stream << std::setfill('0') <<
			std::setw(4) << localTm.tm_year+1900 << "-" <<
			std::setw(2) << localTm.tm_mon+1 << "-" <<
			std::setw(2) << localTm.tm_mday << " " <<
			std::setw(2) << localTm.tm_hour << ":" <<
			std::setw(2) << localTm.tm_min << ":" <<
			std::setw(2) << localTm.tm_sec << "." <<
			std::setw(3) << (milliSeconds.count() % Timestamp::MilliSecondsPerSecond);
		return stream;
	}
}

