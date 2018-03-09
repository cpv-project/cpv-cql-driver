#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlProtocolBatchParameters.hpp"
#include "CqlProtocolConsistency.hpp"
#include "CqlProtocolTimeStamp.hpp"

namespace cql {
	/** Reset to initial state */
	void CqlProtocolBatchParameters::reset() {
		flags_.set(enumValue(CqlQueryParametersFlags::None));
		batchCommand_ = cql::CqlBatchCommand(nullptr);
	}

	/** Set the batch command contains queries and parameters */
	void CqlProtocolBatchParameters::setBatchCommand(CqlBatchCommand&& batchCommand) {
		if (!batchCommand.isValid()) {
			throw CqlLogicException(CQL_CODEINFO,
				"can't set a invalid batch command to batch parameters");
		}
		auto flags = CqlBatchParametersFlags::None;
		if (batchCommand.getSerialConsistencyLevel().second) {
			flags |= CqlBatchParametersFlags::WithSerialConsistency;
		}
		if (batchCommand.getDefaultTimeStamp().second) {
			flags |= CqlBatchParametersFlags::WithDefaultTimestamp;
		}
		flags_.set(enumValue(flags));
		batchCommand_ = std::move(batchCommand);
	}

	/** Encode to binary data */
	void CqlProtocolBatchParameters::encode(seastar::sstring& data) const {
		if (!batchCommand_.isValid()) {
			throw CqlLogicException(CQL_CODEINFO, "invalid(moved) command");
		}
		CqlProtocolConsistency consistency(batchCommand_.getConsistencyLevel());
		consistency.encode(data);
		flags_.encode(data);
		auto flags = getFlags();
		if (enumTrue(flags & CqlBatchParametersFlags::WithSerialConsistency)) {
			CqlProtocolConsistency serialConsistency(
				batchCommand_.getSerialConsistencyLevel().first);
			serialConsistency.encode(data);
		}
		if (enumTrue(flags & CqlBatchParametersFlags::WithDefaultTimestamp)) {
			CqlProtocolTimeStamp timeStamp(batchCommand_.getDefaultTimeStamp().first);
			timeStamp.encode(data);
		}
	}

	/** Decode from binary data */
	void CqlProtocolBatchParameters::decode(const char*& ptr, const char* end) {
		batchCommand_ = CqlBatchCommand();
		CqlProtocolConsistency consistency;
		consistency.decode(ptr, end);
		batchCommand_.setConsistencyLevel(consistency.get());
		flags_.decode(ptr, end);
		auto flags = getFlags();
		if (enumTrue(flags & CqlBatchParametersFlags::WithSerialConsistency)) {
			CqlProtocolConsistency serialConsistency;
			serialConsistency.decode(ptr, end);
			batchCommand_.setSerialConsistencyLevel(serialConsistency.get());
		}
		if (enumTrue(flags & CqlBatchParametersFlags::WithDefaultTimestamp)) {
			CqlProtocolTimeStamp timeStamp;
			timeStamp.decode(ptr, end);
			batchCommand_.setDefaultTimeStamp(timeStamp.get());
		}
	}

	/** Constructor */
	CqlProtocolBatchParameters::CqlProtocolBatchParameters() :
		flags_(enumValue(CqlQueryParametersFlags::None)),
		batchCommand_(nullptr) { }
}

