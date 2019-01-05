#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include "./ProtocolBatchParameters.hpp"
#include "./ProtocolConsistency.hpp"
#include "./ProtocolTimestamp.hpp"

namespace cql {
	/** Reset to initial state */
	void ProtocolBatchParameters::reset() {
		flags_.set(enumValue(BatchParametersFlags::None));
		batchCommand_ = cql::BatchCommand(nullptr);
		batchCommandRef_ = std::ref(batchCommand_);
	}

	/** Set the batch command contains queries and parameters */
	void ProtocolBatchParameters::setBatchCommand(BatchCommand&& batchCommand) {
		// this function will take the ownership of the command
		batchCommand_ = std::move(batchCommand);
		setBatchCommandRef(batchCommand_);
	}

	/** Set the batch command contains queries and parameters */
	void ProtocolBatchParameters::setBatchCommandRef(BatchCommand& batchCommand) {
		// this function won't take the ownership of the command
		if (CQL_UNLIKELY(!batchCommand.isValid())) {
			throw LogicException(CQL_CODEINFO,
				"can't set a invalid batch command to batch parameters");
		} else if (CQL_UNLIKELY(!batchCommand.getConsistency().has_value())) {
			throw LogicException(CQL_CODEINFO,
				"can't set a batch command with empty consistency");
		}
		auto flags = BatchParametersFlags::None;
		if (batchCommand.getSerialConsistency().has_value()) {
			flags |= BatchParametersFlags::WithSerialConsistency;
		}
		if (batchCommand.getDefaultTimestamp().has_value()) {
			flags |= BatchParametersFlags::WithDefaultTimestamp;
		}
		flags_.set(enumValue(flags));
		batchCommandRef_ = std::ref(batchCommand);
	}

	/** Encode to binary data */
	void ProtocolBatchParameters::encode(std::string& data) const {
		auto& batchCommand = batchCommandRef_.get();
		if (!batchCommand.isValid()) {
			throw LogicException(CQL_CODEINFO, "invalid(moved) command");
		}
		ProtocolConsistency consistency(*batchCommand.getConsistency());
		consistency.encode(data);
		flags_.encode(data);
		auto flags = getFlags();
		if (enumTrue(flags & BatchParametersFlags::WithSerialConsistency)) {
			ProtocolConsistency serialConsistency(*batchCommand.getSerialConsistency());
			serialConsistency.encode(data);
		}
		if (enumTrue(flags & BatchParametersFlags::WithDefaultTimestamp)) {
			ProtocolTimestamp timeStamp(*batchCommand.getDefaultTimestamp());
			timeStamp.encode(data);
		}
	}

	/** Decode from binary data */
	void ProtocolBatchParameters::decode(const char*& ptr, const char* end) {
		auto& batchCommand = batchCommandRef_.get();
		batchCommand = BatchCommand();
		ProtocolConsistency consistency;
		consistency.decode(ptr, end);
		batchCommand.setConsistency(consistency.get());
		flags_.decode(ptr, end);
		auto flags = getFlags();
		if (enumTrue(flags & BatchParametersFlags::WithSerialConsistency)) {
			ProtocolConsistency serialConsistency;
			serialConsistency.decode(ptr, end);
			batchCommand.setSerialConsistency(serialConsistency.get());
		}
		if (enumTrue(flags & BatchParametersFlags::WithDefaultTimestamp)) {
			ProtocolTimestamp timeStamp;
			timeStamp.decode(ptr, end);
			batchCommand.setDefaultTimestamp(timeStamp.get());
		}
	}

	/** Constructor */
	ProtocolBatchParameters::ProtocolBatchParameters() :
		flags_(enumValue(BatchParametersFlags::None)),
		batchCommand_(nullptr),
		batchCommandRef_(batchCommand_) { }

	/** Move constructor */
	ProtocolBatchParameters::ProtocolBatchParameters(ProtocolBatchParameters&& other) :
		flags_(other.flags_),
		batchCommand_(nullptr),
		batchCommandRef_(batchCommand_) {
		if (&other.batchCommand_ == &other.batchCommandRef_.get()) {
			// owns the command
			batchCommand_ = std::move(other.batchCommand_);
		} else {
			// not owns the command
			batchCommandRef_ = other.batchCommandRef_;
		}
		other.reset();
	}

	/** Move assignment */
	ProtocolBatchParameters& ProtocolBatchParameters::operator=(ProtocolBatchParameters&& other) {
		if (this != &other) {
			flags_ = other.flags_;
			if (&other.batchCommand_ == &other.batchCommandRef_.get()) {
				// owns the command
				batchCommand_ = std::move(other.batchCommand_);
				batchCommandRef_ = std::ref(batchCommand_);
			} else {
				// not owns the command
				batchCommand_ = BatchCommand(nullptr);
				batchCommandRef_ = other.batchCommandRef_;
			}
			other.reset();
		}
		return *this;
	}
}

