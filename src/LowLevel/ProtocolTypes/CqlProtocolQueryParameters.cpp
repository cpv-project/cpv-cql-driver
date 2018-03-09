#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include <CqlDriver/Common/Exceptions/CqlDecodeException.hpp>
#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include "CqlProtocolQueryParameters.hpp"
#include "CqlProtocolConsistency.hpp"
#include "CqlProtocolInt.hpp"
#include "CqlProtocolShort.hpp"
#include "CqlProtocolBytes.hpp"
#include "CqlProtocolTimeStamp.hpp"

namespace cql {
	/** Reset to initial state */
	void CqlProtocolQueryParameters::reset() {
		flags_.set(enumValue(CqlQueryParametersFlags::None));
		command_ = cql::CqlCommand(nullptr);
	}

	/* Set whether to not receive metadata in result */
	void CqlProtocolQueryParameters::setSkipMetadata(bool value) {
		if (value) {
			flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::SkipMetadata));
		} else {
			flags_.set(enumValue(getFlags() & ~CqlQueryParametersFlags::SkipMetadata));
		}
	}

	/** Set the command contains query and parameters */
	void CqlProtocolQueryParameters::setCommand(CqlCommand&& command) {
		if (!command.isValid()) {
			throw CqlLogicException(CQL_CODEINFO, "can't set a invalid command to query parameters");
		}
		auto flags = getFlags();
		flags &= CqlQueryParametersFlags::SkipMetadata;
		if (command.getParameterCount() > 0) {
			flags |= CqlQueryParametersFlags::WithValues;
		}
		if (command.getPageSize().second) {
			flags |= CqlQueryParametersFlags::WithPageSize;
			if (!command.getPagingState().empty()) {
				flags |= CqlQueryParametersFlags::WithPagingState;
			}
		}
		if (command.getSerialConsistencyLevel().second) {
			flags |= CqlQueryParametersFlags::WithSerialConsistency;
		}
		if (command.getDefaultTimeStamp().second) {
			flags |= CqlQueryParametersFlags::WithDefaultTimestamp;
		}
		flags_.set(enumValue(flags));
		command_ = std::move(command);
	}

	/** Encode to binary data */
	void CqlProtocolQueryParameters::encode(seastar::sstring& data) const {
		if (!command_.isValid()) {
			throw CqlLogicException(CQL_CODEINFO, "invalid(moved) command");
		}
		CqlProtocolConsistency consistency(command_.getConsistencyLevel());
		consistency.encode(data);
		flags_.encode(data);
		auto flags = getFlags();
		if (enumTrue(flags & CqlQueryParametersFlags::WithValues)) {
			CqlProtocolShort parameterCount(command_.getParameterCount());
			if (parameterCount.get() != command_.getParameterCount()) {
				throw CqlLogicException(CQL_CODEINFO, "too many parameters");
			}
			parameterCount.encode(data);
			auto& parameters = command_.getParameters();
			data.append(parameters.data(), parameters.size());
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithPageSize)) {
			CqlProtocolInt pageSize(command_.getPageSize().first);
			pageSize.encode(data);
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithPagingState)) {
			auto& pagingState = command_.getPagingState();
			CqlProtocolInt pagingStateSize(pagingState.size());
			pagingStateSize.encode(data);
			data.append(pagingState.data(), pagingState.size());
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithSerialConsistency)) {
			CqlProtocolConsistency serialConsistency(
				command_.getSerialConsistencyLevel().first);
			serialConsistency.encode(data);
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithDefaultTimestamp)) {
			CqlProtocolTimeStamp timeStamp(command_.getDefaultTimeStamp().first);
			timeStamp.encode(data);
		}
	}

	/** Decode from binary data */
	void CqlProtocolQueryParameters::decode(const char*& ptr, const char* end) {
		command_ = CqlCommand("");
		CqlProtocolConsistency consistency;
		consistency.decode(ptr, end);
		command_.setConsistencyLevel(consistency.get());
		flags_.decode(ptr, end);
		auto flags = getFlags();
		if (enumTrue(flags & CqlQueryParametersFlags::WithValues)) {
			if (enumTrue(flags & CqlQueryParametersFlags::WithNamesForValue)) {
				throw CqlNotImplementedException(CQL_CODEINFO,
					"decode named parameters is unsupported");
			}
			CqlProtocolShort parameterCount;
			CqlProtocolInt parameterSize;
			parameterCount.decode(ptr, end);
			command_.getParameterCount() = parameterCount.get();
			auto& encodedParameters = command_.getParameters();
			for (std::size_t i = 0, j = parameterCount.get(); i < j; ++i) {
				parameterSize.decode(ptr, end);
				parameterSize.encode(encodedParameters);
				if (parameterSize.get() <= 0) {
					continue;
				}
				if (end - ptr < parameterSize.get()) {
					throw CqlDecodeException(CQL_CODEINFO, "length not enough");
				}
				encodedParameters.append(ptr, parameterSize.get());
				ptr += parameterSize.get();
			}
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithPageSize)) {
			CqlProtocolInt pageSize;
			pageSize.decode(ptr, end);
			command_.setPageSize(pageSize.get());
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithPagingState)) {
			CqlProtocolBytes pagingState;
			pagingState.decode(ptr, end);
			command_.setPagingState(std::move(pagingState.get()));
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithSerialConsistency)) {
			CqlProtocolConsistency serialConsistency;
			serialConsistency.decode(ptr, end);
			command_.setSerialConsistencyLevel(serialConsistency.get());
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithDefaultTimestamp)) {
			CqlProtocolTimeStamp timeStamp;
			timeStamp.decode(ptr, end);
			command_.setDefaultTimeStamp(timeStamp.get());
		}
	}

	/** Constructor */
	CqlProtocolQueryParameters::CqlProtocolQueryParameters() :
		flags_(enumValue(CqlQueryParametersFlags::None)),
		command_(nullptr) { }
}

