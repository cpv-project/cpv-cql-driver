#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>
#include "CqlProtocolBatchQuery.hpp"
#include "CqlProtocolShort.hpp"

namespace cql {
	/** Reset to initial state */
	void CqlProtocolBatchQuery::reset() {
		kind_.reset();
		query_.reset();
		preparedQueryId_.reset();
		values_.reset();
	}

	/** Get kind of batch query */
	CqlBatchQueryKind CqlProtocolBatchQuery::getKind() const {
		return static_cast<CqlBatchQueryKind>(kind_.get());
	}
	
	/** Get the CQL query string */
	const seastar::sstring& CqlProtocolBatchQuery::getQuery() const& {
		return query_.get();
	}

	/** Set the CQL query string, also modify the query kind */
	void CqlProtocolBatchQuery::setQuery(const seastar::sstring& query) {
		query_.set(query);
		kind_.set(enumValue(CqlBatchQueryKind::Query));
	}

	/** Set the CQL query string, also modify the query kind */
	void CqlProtocolBatchQuery::setQuery(seastar::sstring&& query) {
		query_.set(std::move(query));
		kind_.set(enumValue(CqlBatchQueryKind::Query));
	}

	/** Get the query id from prepare result */
	const seastar::sstring& CqlProtocolBatchQuery::getPreparedQueryId() const& {
		return preparedQueryId_.get();
	}

	/** Set the query id from prepare result, also modify the query kind */
	void CqlProtocolBatchQuery::setPreparedQueryId(const seastar::sstring& preparedQueryId) {
		preparedQueryId_.set(preparedQueryId);
		kind_.set(enumValue(CqlBatchQueryKind::PreparedQueryId));
	}

	/** Set the query id from prepare result, also modify the query kind */
	void CqlProtocolBatchQuery::setPreparedQueryId(seastar::sstring&& preparedQueryId) {
		preparedQueryId_.set(std::move(preparedQueryId));
		kind_.set(enumValue(CqlBatchQueryKind::PreparedQueryId));
	}

	/** Encode to binary data */
	void CqlProtocolBatchQuery::encode(seastar::sstring& data) const {
		kind_.encode(data);
		if (getKind() == CqlBatchQueryKind::Query) {
			query_.encode(data);
		} else {
			preparedQueryId_.encode(data);
		}
		values_.encode(data);
	}

	/** Decode from binary data */
	void CqlProtocolBatchQuery::decode(const char*& ptr, const char* end) {
		kind_.decode(ptr, end);
		if (getKind() == CqlBatchQueryKind::Query) {
			query_.decode(ptr, end);
		} else {
			preparedQueryId_.decode(ptr, end);
		}
		values_.decode(ptr, end);
	}

	/** Constructor */
	CqlProtocolBatchQuery::CqlProtocolBatchQuery() :
		kind_(),
		query_(),
		preparedQueryId_(),
		values_() { }
}

