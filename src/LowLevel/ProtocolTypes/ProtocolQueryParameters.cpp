#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <CQLDriver/Common/Exceptions/DecodeException.hpp>
#include <CQLDriver/Common/Exceptions/NotImplementedException.hpp>
#include "./ProtocolQueryParameters.hpp"
#include "./ProtocolConsistency.hpp"
#include "./ProtocolInt.hpp"
#include "./ProtocolShort.hpp"
#include "./ProtocolBytes.hpp"
#include "./ProtocolTimestamp.hpp"

namespace cql {
	/** Reset to initial state */
	void ProtocolQueryParameters::reset() {
		flags_.set(enumValue(QueryParametersFlags::None));
		command_ = cql::Command(nullptr);
		commandRef_ = std::ref(command_);
	}

	/* Set whether to not receive metadata in result */
	void ProtocolQueryParameters::setSkipMetadata(bool value) {
		if (value) {
			flags_.set(enumValue(getFlags() | QueryParametersFlags::SkipMetadata));
		} else {
			flags_.set(enumValue(getFlags() & ~QueryParametersFlags::SkipMetadata));
		}
	}

	/** Set the command contains query and parameters */
	void ProtocolQueryParameters::setCommand(Command&& command) {
		// this function will take the ownership of the command
		command_ = std::move(command);
		setCommandRef(command_);
	}

	/** Set the command contains query and parameters */
	void ProtocolQueryParameters::setCommandRef(Command& command) {
		// this function won't take the ownership of the command
		if (!command.isValid()) {
			throw LogicException(CQL_CODEINFO, "can't set a invalid command to query parameters");
		}
		auto flags = getFlags();
		flags &= QueryParametersFlags::SkipMetadata;
		if (command.getParameterCount() > 0) {
			flags |= QueryParametersFlags::WithValues;
		}
		if (command.getPageSize().second) {
			flags |= QueryParametersFlags::WithPageSize;
			if (!command.getPagingState().empty()) {
				flags |= QueryParametersFlags::WithPagingState;
			}
		}
		if (command.getSerialConsistency().second) {
			flags |= QueryParametersFlags::WithSerialConsistency;
		}
		if (command.getDefaultTimestamp().second) {
			flags |= QueryParametersFlags::WithDefaultTimestamp;
		}
		flags_.set(enumValue(flags));
		commandRef_ = std::ref(command);
	}

	/** Encode to binary data */
	void ProtocolQueryParameters::encode(std::string& data) const {
		auto& command = commandRef_.get();
		if (!command.isValid()) {
			throw LogicException(CQL_CODEINFO, "invalid(moved) command");
		}
		ProtocolConsistency consistency(command.getConsistency());
		consistency.encode(data);
		flags_.encode(data);
		auto flags = getFlags();
		if (enumTrue(flags & QueryParametersFlags::WithValues)) {
			ProtocolShort parameterCount(command.getParameterCount());
			if (parameterCount.get() != command.getParameterCount()) {
				throw LogicException(CQL_CODEINFO, "too many parameters");
			}
			parameterCount.encode(data);
			auto& parameters = command.getParameters();
			data.append(parameters.data(), parameters.size());
		}
		if (enumTrue(flags & QueryParametersFlags::WithPageSize)) {
			ProtocolInt pageSize(command.getPageSize().first);
			pageSize.encode(data);
		}
		if (enumTrue(flags & QueryParametersFlags::WithPagingState)) {
			auto& pagingState = command.getPagingState();
			ProtocolInt pagingStateSize(pagingState.size());
			pagingStateSize.encode(data);
			data.append(pagingState.data(), pagingState.size());
		}
		if (enumTrue(flags & QueryParametersFlags::WithSerialConsistency)) {
			ProtocolConsistency serialConsistency(
				command.getSerialConsistency().first);
			serialConsistency.encode(data);
		}
		if (enumTrue(flags & QueryParametersFlags::WithDefaultTimestamp)) {
			ProtocolTimestamp timeStamp(command.getDefaultTimestamp().first);
			timeStamp.encode(data);
		}
	}

	/** Decode from binary data */
	void ProtocolQueryParameters::decode(const char*& ptr, const char* end) {
		auto& command = commandRef_.get();
		command = Command("");
		ProtocolConsistency consistency;
		consistency.decode(ptr, end);
		command.setConsistency(consistency.get());
		flags_.decode(ptr, end);
		auto flags = getFlags();
		if (enumTrue(flags & QueryParametersFlags::WithValues)) {
			if (enumTrue(flags & QueryParametersFlags::WithNamesForValue)) {
				throw NotImplementedException(CQL_CODEINFO,
					"decode named parameters is unsupported");
			}
			ProtocolShort parameterCount;
			ProtocolInt parameterSize;
			parameterCount.decode(ptr, end);
			command.getParameterCount() = parameterCount.get();
			auto& encodedParameters = command.getParameters();
			for (std::size_t i = 0, j = parameterCount.get(); i < j; ++i) {
				parameterSize.decode(ptr, end);
				parameterSize.encode(encodedParameters);
				if (parameterSize.get() <= 0) {
					continue;
				}
				if (end < ptr || end - ptr < parameterSize.get()) {
					throw DecodeException(CQL_CODEINFO, "length not enough");
				}
				encodedParameters.append(ptr, parameterSize.get());
				ptr += parameterSize.get();
			}
		}
		if (enumTrue(flags & QueryParametersFlags::WithPageSize)) {
			ProtocolInt pageSize;
			pageSize.decode(ptr, end);
			command.setPageSize(pageSize.get());
		}
		if (enumTrue(flags & QueryParametersFlags::WithPagingState)) {
			ProtocolBytes pagingState;
			pagingState.decode(ptr, end);
			command.setPagingState(std::move(pagingState.get()));
		}
		if (enumTrue(flags & QueryParametersFlags::WithSerialConsistency)) {
			ProtocolConsistency serialConsistency;
			serialConsistency.decode(ptr, end);
			command.setSerialConsistency(serialConsistency.get());
		}
		if (enumTrue(flags & QueryParametersFlags::WithDefaultTimestamp)) {
			ProtocolTimestamp timeStamp;
			timeStamp.decode(ptr, end);
			command.setDefaultTimestamp(timeStamp.get());
		}
	}

	/** Constructor */
	ProtocolQueryParameters::ProtocolQueryParameters() :
		flags_(enumValue(QueryParametersFlags::None)),
		command_(nullptr),
		commandRef_(command_) { }

	/** Move constructor */
	ProtocolQueryParameters::ProtocolQueryParameters(ProtocolQueryParameters&& other) :
		flags_(other.flags_),
		command_(nullptr),
		commandRef_(command_) {
		if (&other.command_ == &other.commandRef_.get()) {
			// owns the command
			command_ = std::move(other.command_);
		} else {
			// not owns the command
			commandRef_ = other.commandRef_;
		}
		other.reset();
	}

	/** Move assignment */
	ProtocolQueryParameters& ProtocolQueryParameters::operator=(ProtocolQueryParameters&& other) {
		if (this != &other) {
			flags_ = other.flags_;
			if (&other.command_ == &other.commandRef_.get()) {
				// owns the command
				command_ = std::move(other.command_);
				commandRef_ = std::ref(command_);
			} else {
				// not owns the command
				command_ = Command(nullptr);
				commandRef_ = other.commandRef_;
			}
			other.reset();
		}
		return *this;
	}
}

