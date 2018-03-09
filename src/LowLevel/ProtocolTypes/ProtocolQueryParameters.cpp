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
		command_ = std::move(command);
	}

	/** Encode to binary data */
	void ProtocolQueryParameters::encode(seastar::sstring& data) const {
		if (!command_.isValid()) {
			throw LogicException(CQL_CODEINFO, "invalid(moved) command");
		}
		ProtocolConsistency consistency(command_.getConsistency());
		consistency.encode(data);
		flags_.encode(data);
		auto flags = getFlags();
		if (enumTrue(flags & QueryParametersFlags::WithValues)) {
			ProtocolShort parameterCount(command_.getParameterCount());
			if (parameterCount.get() != command_.getParameterCount()) {
				throw LogicException(CQL_CODEINFO, "too many parameters");
			}
			parameterCount.encode(data);
			auto& parameters = command_.getParameters();
			data.append(parameters.data(), parameters.size());
		}
		if (enumTrue(flags & QueryParametersFlags::WithPageSize)) {
			ProtocolInt pageSize(command_.getPageSize().first);
			pageSize.encode(data);
		}
		if (enumTrue(flags & QueryParametersFlags::WithPagingState)) {
			auto& pagingState = command_.getPagingState();
			ProtocolInt pagingStateSize(pagingState.size());
			pagingStateSize.encode(data);
			data.append(pagingState.data(), pagingState.size());
		}
		if (enumTrue(flags & QueryParametersFlags::WithSerialConsistency)) {
			ProtocolConsistency serialConsistency(
				command_.getSerialConsistency().first);
			serialConsistency.encode(data);
		}
		if (enumTrue(flags & QueryParametersFlags::WithDefaultTimestamp)) {
			ProtocolTimestamp timeStamp(command_.getDefaultTimestamp().first);
			timeStamp.encode(data);
		}
	}

	/** Decode from binary data */
	void ProtocolQueryParameters::decode(const char*& ptr, const char* end) {
		command_ = Command("");
		ProtocolConsistency consistency;
		consistency.decode(ptr, end);
		command_.setConsistency(consistency.get());
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
			command_.getParameterCount() = parameterCount.get();
			auto& encodedParameters = command_.getParameters();
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
			command_.setPageSize(pageSize.get());
		}
		if (enumTrue(flags & QueryParametersFlags::WithPagingState)) {
			ProtocolBytes pagingState;
			pagingState.decode(ptr, end);
			command_.setPagingState(std::move(pagingState.get()));
		}
		if (enumTrue(flags & QueryParametersFlags::WithSerialConsistency)) {
			ProtocolConsistency serialConsistency;
			serialConsistency.decode(ptr, end);
			command_.setSerialConsistency(serialConsistency.get());
		}
		if (enumTrue(flags & QueryParametersFlags::WithDefaultTimestamp)) {
			ProtocolTimestamp timeStamp;
			timeStamp.decode(ptr, end);
			command_.setDefaultTimestamp(timeStamp.get());
		}
	}

	/** Constructor */
	ProtocolQueryParameters::ProtocolQueryParameters() :
		flags_(enumValue(QueryParametersFlags::None)),
		command_(nullptr) { }
}

