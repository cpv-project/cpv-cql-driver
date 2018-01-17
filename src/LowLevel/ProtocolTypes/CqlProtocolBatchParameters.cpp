#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlProtocolBatchParameters.hpp"
#include "CqlProtocolShort.hpp"

namespace cql {
	/** Reset to initial state */
	void CqlProtocolBatchParameters::reset() {
		consistency_.reset();
		flags_.set(enumValue(CqlBatchParametersFlags::None));
	}

	/** Set the consistency level for the serial phase of conditional update */
	void CqlProtocolBatchParameters::setSerialConsistency(CqlConsistencyLevel serialConsistency) {
		serialConsistency_.set(serialConsistency);
		flags_.set(enumValue(getFlags() | CqlBatchParametersFlags::WithSerialConsistency));
	}

	/** Set the default timestamp */
	void CqlProtocolBatchParameters::setDefaultTimestamp(std::uint64_t timestamp) {
		defaultTimestamp_.set(timestamp);
		flags_.set(enumValue(getFlags() | CqlBatchParametersFlags::WithDefaultTimestamp));
	}

	/** Encode to binary data */
	void CqlProtocolBatchParameters::encode(seastar::sstring& data) const {
		auto flags = getFlags();
		consistency_.encode(data);
		flags_.encode(data);
		if (enumTrue(flags & CqlBatchParametersFlags::WithSerialConsistency)) {
			serialConsistency_.encode(data);
		}
		if (enumTrue(flags & CqlBatchParametersFlags::WithDefaultTimestamp)) {
			defaultTimestamp_.encode(data);
		}
	}

	/** Decode from binary data */
	void CqlProtocolBatchParameters::decode(const char*& ptr, const char* end) {
		consistency_.decode(ptr, end);
		flags_.decode(ptr, end);
		auto flags = getFlags();
		if (enumTrue(flags & CqlBatchParametersFlags::WithSerialConsistency)) {
			serialConsistency_.decode(ptr, end);
		}
		if (enumTrue(flags & CqlBatchParametersFlags::WithDefaultTimestamp)) {
			defaultTimestamp_.decode(ptr, end);
		}
	}

	/** Constructor */
	CqlProtocolBatchParameters::CqlProtocolBatchParameters() :
		consistency_(),
		flags_(),
		serialConsistency_(),
		defaultTimestamp_() { }
}

