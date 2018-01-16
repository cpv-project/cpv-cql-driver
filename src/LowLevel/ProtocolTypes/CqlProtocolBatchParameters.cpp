#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlProtocolBatchParameters.hpp"
#include "CqlProtocolShort.hpp"

namespace cql {
	void CqlProtocolBatchParameters::reset() {
		consistency_.reset();
		flags_.set(enumValue(CqlBatchParametersFlags::None));
	}

	CqlConsistencyLevel CqlProtocolBatchParameters::getConsistency() const {
		return consistency_.get();
	}

	void CqlProtocolBatchParameters::setConsistency(CqlConsistencyLevel consistency) {
		consistency_.set(consistency);
	}

	CqlBatchParametersFlags CqlProtocolBatchParameters::getFlags() const {
		return static_cast<CqlBatchParametersFlags>(flags_.get());
	}

	CqlConsistencyLevel CqlProtocolBatchParameters::getSerialConsistency() const {
		return serialConsistency_.get();
	}

	void CqlProtocolBatchParameters::setSerialConsistency(CqlConsistencyLevel serialConsistency) {
		serialConsistency_.set(serialConsistency);
		flags_.set(enumValue(getFlags() | CqlBatchParametersFlags::WithSerialConsistency));
	}

	std::uint64_t CqlProtocolBatchParameters::getDefaultTimestamp() const {
		return defaultTimestamp_.get();
	}

	void CqlProtocolBatchParameters::setDefaultTimestamp(std::uint64_t timestamp) {
		defaultTimestamp_.set(timestamp);
		flags_.set(enumValue(getFlags() | CqlBatchParametersFlags::WithDefaultTimestamp));
	}

	const seastar::sstring& CqlProtocolBatchParameters::getKeySpace() const& {
		return keySpace_.get();
	}

	void CqlProtocolBatchParameters::setKeySpace(const seastar::sstring& keySpace) {
		keySpace_.set(keySpace.data(), keySpace.size());
		flags_.set(enumValue(getFlags() | CqlBatchParametersFlags::WithKeySpace));
	}

	void CqlProtocolBatchParameters::setKeySpace(seastar::sstring&& keySpace) {
		keySpace_.set(CqlProtocolStringState::Normal);
		keySpace_.get() = std::move(keySpace);
		flags_.set(enumValue(getFlags() | CqlBatchParametersFlags::WithKeySpace));
	}

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
		if (enumTrue(flags & CqlBatchParametersFlags::WithKeySpace)) {
			keySpace_.encode(data);
		}
	}

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
		if (enumTrue(flags & CqlBatchParametersFlags::WithKeySpace)) {
			keySpace_.decode(ptr, end);
		}
	}

	CqlProtocolBatchParameters::CqlProtocolBatchParameters() :
		consistency_(),
		flags_(),
		serialConsistency_(),
		defaultTimestamp_(),
		keySpace_() { }
}

