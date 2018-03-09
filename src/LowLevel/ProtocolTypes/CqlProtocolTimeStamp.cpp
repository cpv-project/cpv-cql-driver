#include "CqlProtocolTimeStamp.hpp"

namespace cql {
	/** Get the timestamp value */
	std::chrono::system_clock::time_point CqlProtocolTimeStamp::get() const {
		return std::chrono::system_clock::from_time_t(0) +
			std::chrono::microseconds(CqlProtocolIntegerBase::get());
	}

	/** Set the timestamp value */
	void CqlProtocolTimeStamp::set(std::chrono::system_clock::time_point value) {
		CqlProtocolIntegerBase::set(std::chrono::duration_cast<
			std::chrono::microseconds>(value.time_since_epoch()).count());
	}

	/** Constructor */
	CqlProtocolTimeStamp::CqlProtocolTimeStamp() :
		CqlProtocolIntegerBase(0) { }

	/** Constructor */
	CqlProtocolTimeStamp::CqlProtocolTimeStamp(
		std::chrono::system_clock::time_point value) :
		CqlProtocolIntegerBase(0) {
		set(value);
	}
}

