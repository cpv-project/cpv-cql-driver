#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include <CqlDriver/Common/Exceptions/CqlNotImplementedException.hpp>
#include "CqlProtocolQueryParameters.hpp"
#include "CqlProtocolShort.hpp"

namespace cql {
	/** Reset to initial state */
	void CqlProtocolQueryParameters::reset() {
		consistency_.reset();
		flags_.set(enumValue(CqlQueryParametersFlags::None));
	}

	/* Set whether to no receive metadata in result */
	void CqlProtocolQueryParameters::setSkipMetadata(bool value) {
		if (value) {
			flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::SkipMetadata));
		} else {
			flags_.set(enumValue(getFlags() & ~CqlQueryParametersFlags::SkipMetadata));
		}
	}

	/** Set the named query parameters */
	void CqlProtocolQueryParameters::setNameAndValues(const NameAndValuesType& nameAndValues) {
		nameAndValues_.set(nameAndValues);
		flags_.set(enumValue(
			getFlags() | CqlQueryParametersFlags::WithValues |
			CqlQueryParametersFlags::WithNamesForValue));
	}

	/** Set the named query parameters */
	void CqlProtocolQueryParameters::setNameAndValues(NameAndValuesType&& nameAndValues) {
		nameAndValues_.set(std::move(nameAndValues));
		flags_.set(enumValue(
			getFlags() | CqlQueryParametersFlags::WithValues |
			CqlQueryParametersFlags::WithNamesForValue));
	}

	/** Set the unnamed query parameters */
	void CqlProtocolQueryParameters::setValues(const std::vector<CqlProtocolValue>& values) {
		values_.get() = values;
		flags_.set(enumValue(
			(getFlags() | CqlQueryParametersFlags::WithValues) &
			(~CqlQueryParametersFlags::WithNamesForValue)));
	}

	/** Set the unnamed query parameters */
	void CqlProtocolQueryParameters::setValues(std::vector<CqlProtocolValue>&& values) {
		values_.get() = std::move(values);
		flags_.set(enumValue(
			(getFlags() | CqlQueryParametersFlags::WithValues) &
			(~CqlQueryParametersFlags::WithNamesForValue)));
	}

	/** Set the page size of the result */
	void CqlProtocolQueryParameters::setPageSize(std::size_t pageSize) {
		pageSize_.set(static_cast<decltype(pageSize_.get())>(pageSize));
		if (pageSize_.get() < 0 || pageSize_.get() != pageSize) {
			throw CqlLogicException(CQL_CODEINFO, "page size overflow");
		}
		flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::WithPageSize));
	}

	/** Set the paging state */
	void CqlProtocolQueryParameters::setPagingState(const seastar::sstring& pagingState) {
		pagingState_.set(pagingState);
		flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::WithPagingState));
	}

	/** Set the paging state */
	void CqlProtocolQueryParameters::setPagingState(seastar::sstring&& pagingState) {
		pagingState_.set(std::move(pagingState));
		flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::WithPagingState));
	}

	/** Set the consistency level for the serial phase of conditional update */
	void CqlProtocolQueryParameters::setSerialConsistency(CqlConsistencyLevel serialConsistency) {
		serialConsistency_.set(serialConsistency);
		flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::WithSerialConsistency));
	}

	/** Set the default timestamp */
	void CqlProtocolQueryParameters::setDefaultTimestamp(std::uint64_t timestamp) {
		defaultTimestamp_.set(timestamp);
		flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::WithDefaultTimestamp));
	}

	/** Encode to binary data */
	void CqlProtocolQueryParameters::encode(seastar::sstring& data) const {
		auto flags = getFlags();
		consistency_.encode(data);
		flags_.encode(data);
		if (enumTrue(flags & CqlQueryParametersFlags::WithValues)) {
			if (enumTrue(flags & CqlQueryParametersFlags::WithNamesForValue)) {
				nameAndValues_.encode(data);
			} else {
				values_.encode(data);
			}
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithPageSize)) {
			pageSize_.encode(data);
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithPagingState)) {
			pagingState_.encode(data);
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithSerialConsistency)) {
			serialConsistency_.encode(data);
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithDefaultTimestamp)) {
			defaultTimestamp_.encode(data);
		}
	}

	/** Decode from binary data */
	void CqlProtocolQueryParameters::decode(const char*& ptr, const char* end) {
		consistency_.decode(ptr, end);
		flags_.decode(ptr, end);
		auto flags = getFlags();
		if (enumTrue(flags & CqlQueryParametersFlags::WithValues)) {
			if (enumTrue(flags & CqlQueryParametersFlags::WithNamesForValue)) {
				nameAndValues_.decode(ptr, end);
			} else {
				values_.decode(ptr, end);
			}
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithPageSize)) {
			pageSize_.decode(ptr, end);
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithPagingState)) {
			pagingState_.decode(ptr, end);
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithSerialConsistency)) {
			serialConsistency_.decode(ptr, end);
		}
		if (enumTrue(flags & CqlQueryParametersFlags::WithDefaultTimestamp)) {
			defaultTimestamp_.decode(ptr, end);
		}
	}

	/** Constructor */
	CqlProtocolQueryParameters::CqlProtocolQueryParameters() :
		consistency_(),
		flags_(),
		values_(),
		nameAndValues_(),
		pageSize_(),
		pagingState_(),
		serialConsistency_(),
		defaultTimestamp_() { }
}

