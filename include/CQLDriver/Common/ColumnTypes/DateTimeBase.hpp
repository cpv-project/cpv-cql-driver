#pragma once
#include <chrono>
#include <cstring>
#include <seastar/core/byteorder.hh>
#include "../Exceptions/DecodeException.hpp"
#include "./DateTimeOperations.hpp"

namespace cql {
	/**
	 * Base class of date time types, didn't include encode and decode functions.
	 * The time point load from and store to this class should always expressed in utc.
	 */
	class DateTimeBase {
	public:
		using UnderlyingType = std::chrono::system_clock::time_point;

		/** Get the time point value */
		std::chrono::system_clock::time_point get() const& { return value_; }

		/** Get the mutable time point value */
		std::chrono::system_clock::time_point& get() & { return value_; }

		/** Set the time point value */
		void set(std::chrono::system_clock::time_point utcTimePoint) {
			value_ = utcTimePoint;
		}

		/** Set the time point value from time_t */
		void set(std::time_t utcTime) {
			value_ = std::chrono::system_clock::from_time_t(utcTime);
		}

		/** Set the time point value from std::tm expressed in local timezone */
		void set(std::tm localTm) {
			value_ = std::chrono::system_clock::from_time_t(std::mktime(&localTm));
		}

		/** Reset to initial state */
		void reset() {
			value_ = std::chrono::system_clock::from_time_t(0);
		}

		/** Constructors */
		// cppcheck-suppress noExplicitConstructor
		DateTimeBase() : value_(std::chrono::system_clock::from_time_t(0)) { }
		template <class... Args>
		// cppcheck-suppress noExplicitConstructor
		DateTimeBase(Args&&... args) : value_() {
			set(std::forward<Args>(args)...);
		}

		/** Allow assign from whatever function "set" would take */
		template <class... Args>
		void operator=(Args&&... args) {
			set(std::forward<Args>(args)...);
		}

		/** Allow cast to time point implicitly */
		operator std::chrono::system_clock::time_point() const {
			return value_;
		}

		/** Allow cast to time_t implicitly */
		explicit operator std::time_t() const {
			// explicit is used to prevent unintended conversion,
			// when operator std::time_t and operator std::chrono::nanoseconds both exists,
			// gcc 9 will pick std::time_t when using static_cast<std::chrono::nanoseconds>,
			// it may be a compiler bug that doesn't exists in gcc 7,
			// anyway this type shouldn't convert to an integer implicitly.
			return std::chrono::system_clock::to_time_t(value_);
		}

		/** Allow cast to std::tm expressed in local timezone implicitly */
		operator std::tm() const {
			std::time_t utcTime = static_cast<std::time_t>(*this);
			std::tm localTm = {};
			::localtime_r(&utcTime, &localTm);
			return localTm;
		}

	protected:
		std::chrono::system_clock::time_point value_;
	};
}

