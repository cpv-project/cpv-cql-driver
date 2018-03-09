#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include "./ProtocolBatchParameters.hpp"
#include "./ProtocolConsistency.hpp"
#include "./ProtocolTimestamp.hpp"

namespace cql {
	/** Reset to initial state */
	void ProtocolBatchParameters::reset() {
		flags_.set(enumValue(QueryParametersFlags::None));
		batchCommand_ = cql::BatchCommand(nullptr);
	}

	/** Set the batch command contains queries and parameters */
	void ProtocolBatchParameters::setBatchCommand(BatchCommand&& batchCommand) {
		if (!batchCommand.isValid()) {
			throw LogicException(CQL_CODEINFO,
				"can't set a invalid batch command to batch parameters");
		}
		auto flags = BatchParametersFlags::None;
		if (batchCommand.getSerialConsistency().second) {
			flags |= BatchParametersFlags::WithSerialConsistency;
		}
		if (batchCommand.getDefaultTimestamp().second) {
			flags |= BatchParametersFlags::WithDefaultTimestamp;
		}
		flags_.set(enumValue(flags));
		batchCommand_ = std::move(batchCommand);
	}

	/** Encode to binary data */
	void ProtocolBatchParameters::encode(seastar::sstring& data) const {
		if (!batchCommand_.isValid()) {
			throw LogicException(CQL_CODEINFO, "invalid(moved) command");
		}
		ProtocolConsistency consistency(batchCommand_.getConsistency());
		consistency.encode(data);
		flags_.encode(data);
		auto flags = getFlags();
		if (enumTrue(flags & BatchParametersFlags::WithSerialConsistency)) {
			ProtocolConsistency serialConsistency(
				batchCommand_.getSerialConsistency().first);
			serialConsistency.encode(data);
		}
		if (enumTrue(flags & BatchParametersFlags::WithDefaultTimestamp)) {
			ProtocolTimestamp timeStamp(batchCommand_.getDefaultTimestamp().first);
			timeStamp.encode(data);
		}
	}

	/** Decode from binary data */
	void ProtocolBatchParameters::decode(const char*& ptr, const char* end) {
		batchCommand_ = BatchCommand();
		ProtocolConsistency consistency;
		consistency.decode(ptr, end);
		batchCommand_.setConsistency(consistency.get());
		flags_.decode(ptr, end);
		auto flags = getFlags();
		if (enumTrue(flags & BatchParametersFlags::WithSerialConsistency)) {
			ProtocolConsistency serialConsistency;
			serialConsistency.decode(ptr, end);
			batchCommand_.setSerialConsistency(serialConsistency.get());
		}
		if (enumTrue(flags & BatchParametersFlags::WithDefaultTimestamp)) {
			ProtocolTimestamp timeStamp;
			timeStamp.decode(ptr, end);
			batchCommand_.setDefaultTimestamp(timeStamp.get());
		}
	}

	/** Constructor */
	ProtocolBatchParameters::ProtocolBatchParameters() :
		flags_(enumValue(QueryParametersFlags::None)),
		batchCommand_(nullptr) { }
}

