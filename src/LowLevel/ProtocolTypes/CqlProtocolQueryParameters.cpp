#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlProtocolQueryParameters.hpp"
#include "CqlProtocolShort.hpp"

namespace cql {
	void CqlProtocolQueryParameters::reset() {
		consistency_.reset();
		flags_.set(enumValue(CqlQueryParametersFlags::None));
	}

	CqlConsistencyLevel CqlProtocolQueryParameters::getConsistency() const {
		return consistency_.get();
	}

	void CqlProtocolQueryParameters::setConsistency(CqlConsistencyLevel consistency) {
		consistency_.set(consistency);
	}

	CqlQueryParametersFlags CqlProtocolQueryParameters::getFlags() const {
		return static_cast<CqlQueryParametersFlags>(flags_.get());
	}

	void CqlProtocolQueryParameters::setSkipMetadata(bool value) {
		if (value) {
			flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::SkipMetadata));
		} else {
			flags_.set(enumValue(getFlags() & ~CqlQueryParametersFlags::SkipMetadata));
		}
	}

	const std::vector<CqlProtocolString>& CqlProtocolQueryParameters::getNames() const& {
		return names_;
	}

	const std::vector<CqlProtocolValue>& CqlProtocolQueryParameters::getValues() const& {
		return values_;
	}

	void CqlProtocolQueryParameters::setValues(const std::vector<CqlProtocolValue>& values) {
		values_ = values;
		flags_.set(enumValue(
			(getFlags() | CqlQueryParametersFlags::WithValues) &
			(~CqlQueryParametersFlags::WithNamesForValue)));
	}

	void CqlProtocolQueryParameters::setValues(std::vector<CqlProtocolValue>&& values) {
		values_ = std::move(values);
		flags_.set(enumValue(
			(getFlags() | CqlQueryParametersFlags::WithValues) &
			(~CqlQueryParametersFlags::WithNamesForValue)));
	}

	void CqlProtocolQueryParameters::setNameAndValues(
		const std::vector<CqlProtocolString>& names, const std::vector<CqlProtocolValue>& values) {
		names_ = names;
		values_ = values;
		flags_.set(enumValue(
			getFlags() | CqlQueryParametersFlags::WithValues |
			CqlQueryParametersFlags::WithNamesForValue));
	}

	void CqlProtocolQueryParameters::setNameAndValues(
		std::vector<CqlProtocolString>&& names, std::vector<CqlProtocolValue>&& values) {
		names_ = std::move(names);
		values_ = std::move(values);
		flags_.set(enumValue(
			getFlags() | CqlQueryParametersFlags::WithValues |
			CqlQueryParametersFlags::WithNamesForValue));
	}

	std::size_t CqlProtocolQueryParameters::getPageSize() const {
		return static_cast<std::size_t>(pageSize_.get());
	}

	void CqlProtocolQueryParameters::setPageSize(std::size_t pageSize) {
		pageSize_.set(static_cast<decltype(pageSize_.get())>(pageSize));
		if (pageSize < 0 || pageSize_.get() != pageSize) {
			throw CqlLogicException(CQL_CODEINFO, "page size overflow");
		}
		flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::WithPageSize));
	}

	const seastar::sstring& CqlProtocolQueryParameters::getPagingState() const& {
		return pagingState_.get();
	}

	void CqlProtocolQueryParameters::setPagingState(const seastar::sstring& pagingState) {
		pagingState_.set(pagingState.data(), pagingState.size());
		flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::WithPagingState));
	}

	void CqlProtocolQueryParameters::setPagingState(seastar::sstring&& pagingState) {
		pagingState_.set(CqlProtocolBytesState::Normal);
		pagingState_.get() = std::move(pagingState);
		flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::WithPagingState));
	}

	CqlConsistencyLevel CqlProtocolQueryParameters::getSerialConsistency() const {
		return serialConsistency_.get();
	}

	void CqlProtocolQueryParameters::setSerialConsistency(CqlConsistencyLevel serialConsistency) {
		serialConsistency_.set(serialConsistency);
		flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::WithSerialConsistency));
	}

	std::uint64_t CqlProtocolQueryParameters::getDefaultTimestamp() const {
		return defaultTimestamp_.get();
	}

	void CqlProtocolQueryParameters::setDefaultTimestamp(std::uint64_t timestamp) {
		defaultTimestamp_.set(timestamp);
		flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::WithDefaultTimestamp));
	}

	const seastar::sstring& CqlProtocolQueryParameters::getKeySpace() const& {
		return keySpace_.get();
	}

	void CqlProtocolQueryParameters::setKeySpace(const seastar::sstring& keySpace) {
		keySpace_.set(keySpace.data(), keySpace.size());
		flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::WithKeySpace));
	}

	void CqlProtocolQueryParameters::setKeySpace(seastar::sstring&& keySpace) {
		keySpace_.set(CqlProtocolStringState::Normal);
		keySpace_.get() = std::move(keySpace);
		flags_.set(enumValue(getFlags() | CqlQueryParametersFlags::WithKeySpace));
	}

	void CqlProtocolQueryParameters::encode(seastar::sstring& data) const {
		auto flags = getFlags();
		consistency_.encode(data);
		flags_.encode(data);
		if (enumTrue(flags & CqlQueryParametersFlags::WithValues)) {
			CqlProtocolShort valuesCount(values_.size());
			if (valuesCount.get() != values_.size()) {
				throw CqlLogicException(CQL_CODEINFO, "too many values cause overflow");
			}
			valuesCount.encode(data);
			if (enumTrue(flags & CqlQueryParametersFlags::WithNamesForValue)) {
				if (names_.size() != values_.size()) {
					throw CqlLogicException(CQL_CODEINFO, "names size not equal to values size");
				}
				for (std::size_t i = 0; i < values_.size(); ++i) {
					names_[i].encode(data);
					values_[i].encode(data);
				}
			} else {
				for (const auto& value : values_) {
					value.encode(data);
				}
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
		if (enumTrue(flags & CqlQueryParametersFlags::WithKeySpace)) {
			keySpace_.encode(data);
		}
	}

	void CqlProtocolQueryParameters::decode(const char*& ptr, const char* end) {
		consistency_.decode(ptr, end);
		flags_.decode(ptr, end);
		auto flags = getFlags();
		if (enumTrue(flags & CqlQueryParametersFlags::WithValues)) {
			CqlProtocolShort valuesCount(values_.size());
			valuesCount.decode(ptr, end);
			if (enumTrue(flags & CqlQueryParametersFlags::WithNamesForValue)) {
				names_.resize(valuesCount.get());
				values_.resize(valuesCount.get());
				for (std::size_t i = 0; i < values_.size(); ++i) {
					names_[i].decode(ptr, end);
					values_[i].decode(ptr, end);
				}
			} else {
				// didn't touch names_, for future reuse
				values_.resize(valuesCount.get());
				for (auto& value : values_) {
					value.decode(ptr, end);
				}
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
		if (enumTrue(flags & CqlQueryParametersFlags::WithKeySpace)) {
			keySpace_.decode(ptr, end);
		}
	}

	CqlProtocolQueryParameters::CqlProtocolQueryParameters() :
		consistency_(),
		flags_(),
		names_(),
		values_(),
		pageSize_(),
		pagingState_(),
		serialConsistency_(),
		defaultTimestamp_(),
		keySpace_() { }
}

