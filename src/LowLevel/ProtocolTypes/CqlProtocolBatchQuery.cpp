#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlProtocolBatchQuery.hpp"
#include "CqlProtocolShort.hpp"

namespace cql {
	void CqlProtocolBatchQuery::reset() {
		kind_.reset();
		query_.reset();
		preparedQueryId_.reset();
		values_.clear();
	}

	CqlBatchQueryKind CqlProtocolBatchQuery::getKind() const {
		return static_cast<CqlBatchQueryKind>(kind_.get());
	}
	
	const seastar::sstring& CqlProtocolBatchQuery::getQuery() const& {
		return query_.get();
	}

	void CqlProtocolBatchQuery::setQuery(const seastar::sstring& query) {
		query_.set(query.data(), query.size());
		kind_.set(enumValue(CqlBatchQueryKind::Query));
	}

	void CqlProtocolBatchQuery::setQuery(seastar::sstring&& query) {
		query_.set(CqlProtocolLongStringState::Normal);
		query_.get() = std::move(query);
		kind_.set(enumValue(CqlBatchQueryKind::Query));
	}

	const seastar::sstring& CqlProtocolBatchQuery::getPreparedQueryId() const& {
		return preparedQueryId_.get();
	}

	void CqlProtocolBatchQuery::setPreparedQueryId(const seastar::sstring& preparedQueryId) {
		preparedQueryId_.set(preparedQueryId.data(), preparedQueryId.size());
		kind_.set(enumValue(CqlBatchQueryKind::PreparedQueryId));
	}

	void CqlProtocolBatchQuery::setPreparedQueryId(seastar::sstring&& preparedQueryId) {
		preparedQueryId_.set(CqlProtocolShortBytesState::Normal);
		preparedQueryId_.get() = std::move(preparedQueryId);
		kind_.set(enumValue(CqlBatchQueryKind::PreparedQueryId));
	}

	const std::vector<CqlProtocolValue>& CqlProtocolBatchQuery::getValues() const& {
		return values_;
	}

	void CqlProtocolBatchQuery::setValues(const std::vector<CqlProtocolValue>& values) {
		values_ = values;
	}

	void CqlProtocolBatchQuery::setValues(std::vector<CqlProtocolValue>&& values) {
		values_ = std::move(values);
	}

	void CqlProtocolBatchQuery::encode(seastar::sstring& data) const {
		kind_.encode(data);
		if (getKind() == CqlBatchQueryKind::Query) {
			query_.encode(data);
		} else {
			preparedQueryId_.encode(data);
		}
		CqlProtocolShort valuesCount(values_.size());
		if (valuesCount.get() != values_.size()) {
			throw CqlLogicException(CQL_CODEINFO, "too many values cause overflow");
		}
		valuesCount.encode(data);
		for (const auto& value : values_) {
			value.encode(data);
		}
	}

	void CqlProtocolBatchQuery::decode(const char*& ptr, const char* end) {
		kind_.decode(ptr, end);
		if (getKind() == CqlBatchQueryKind::Query) {
			query_.decode(ptr, end);
		} else {
			preparedQueryId_.decode(ptr, end);
		}
		CqlProtocolShort valuesCount;
		valuesCount.decode(ptr, end);
		values_.resize(valuesCount.get());
		for (auto& value : values_) {
			value.decode(ptr, end);
		}
	}

	CqlProtocolBatchQuery::CqlProtocolBatchQuery() :
		kind_(),
		query_(),
		preparedQueryId_(),
		values_() { }
}

