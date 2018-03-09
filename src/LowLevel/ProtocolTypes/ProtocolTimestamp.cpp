#include "./ProtocolTimestamp.hpp"

namespace cql {
	/** Get the timestamp value */
	std::chrono::system_clock::time_point ProtocolTimestamp::get() const {
		return std::chrono::system_clock::from_time_t(0) +
			std::chrono::microseconds(ProtocolIntegerBase::get());
	}

	/** Set the timestamp value */
	void ProtocolTimestamp::set(std::chrono::system_clock::time_point value) {
		ProtocolIntegerBase::set(std::chrono::duration_cast<
			std::chrono::microseconds>(value.time_since_epoch()).count());
	}

	/** Constructor */
	ProtocolTimestamp::ProtocolTimestamp() :
		ProtocolIntegerBase(0) { }

	/** Constructor */
	ProtocolTimestamp::ProtocolTimestamp(
		std::chrono::system_clock::time_point value) :
		ProtocolIntegerBase(0) {
		set(value);
	}
}

