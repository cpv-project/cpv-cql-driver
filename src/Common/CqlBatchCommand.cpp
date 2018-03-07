#include <CqlDriver/Common/CqlBatchCommand.hpp>
#include <CqlDriver/Common/Exceptions/CqlLogicException.hpp>

namespace cql {
	/** Defines members of CqlBatchCommand */
	class CqlBatchCommandData {
	public:
		struct QueryData {
			std::pair<const char*, std::size_t> queryCStr;
			seastar::sstring queryStr;
			std::vector<std::pair<std::size_t, seastar::sstring>> parameterSets;

			QueryData() :
				queryCStr(),
				queryStr(),
				parameterSets() { }

			QueryData(const char* query, std::size_t size) :
				queryCStr(query, size),
				queryStr(),
				parameterSets() { }

			explicit QueryData(seastar::sstring&& query) :
				queryCStr(nullptr, 0),
				queryStr(std::move(query)),
				parameterSets() { }
		};

		CqlConsistencyLevel consistencyLevel;
		std::vector<QueryData> queries;

		CqlBatchCommandData() :
			consistencyLevel(),
			queries() { }

		// cppcheck-suppress functionStatic
		void freeResources() { }

		void reset() {
			consistencyLevel = CqlConsistencyLevel::Any;
			queries.resize(0);
		}
	};

	/** Check whether this is a valid command (will be false if moved) */
	bool CqlBatchCommand::isValid() const {
		return data_ != nullptr;
	}

	/** Set the consistency level of this batch */
	CqlBatchCommand& CqlBatchCommand::setConsistencyLevel(CqlConsistencyLevel consistencyLevel) & {
		data_->consistencyLevel = consistencyLevel;
		return *this;
	}

	/** Add a new query to this batch */
	CqlBatchCommand& CqlBatchCommand::addQuery(seastar::sstring&& query) & {
		data_->queries.emplace_back(std::move(query));
		return *this;
	}

	/** Add a new query to this batch */
	CqlBatchCommand& CqlBatchCommand::addQuery(const char* query, std::size_t size) & {
		data_->queries.emplace_back(query, size);
		return *this;
	}

	/** Open a new parameter set explicitly of the last query */
	CqlBatchCommand& CqlBatchCommand::openParameterSet() & {
		if (data_->queries.empty()) {
			throw cql::CqlLogicException(CQL_CODEINFO,
				"please call addQuery before openParameterSet");
		}
		data_->queries.back().parameterSets.emplace_back();
		return *this;
	}

	/** Get the consistency level of this batch */
	CqlConsistencyLevel CqlBatchCommand::getConsistencyLevel() const {
		return data_->consistencyLevel;
	}

	/** Get how many queries in this batch */
	std::size_t CqlBatchCommand::getQueryCount() const {
		return data_->queries.size();
	}

	/** Get the query string by index */
	std::pair<const char*, std::size_t> CqlBatchCommand::getQuery(std::size_t index) const& {
		auto& queryData = data_->queries.at(index);
		if (queryData.queryCStr.first != nullptr) {
			return queryData.queryCStr;
		} else {
			return { queryData.queryStr.c_str(), queryData.queryStr.size() };
		}
	}

	/** Get the parameter sets by index */
	const CqlBatchCommand::ParameterSetsType&
		CqlBatchCommand::getParameterSets(std::size_t index) const& {
		return data_->queries.at(index).parameterSets;
	}

	/** Constructor */
	CqlBatchCommand::CqlBatchCommand() :
		data_(makeObject<CqlBatchCommandData>()) { }

	/** Get the mutable count of parameters of the last parameter set */
	std::size_t& CqlBatchCommand::getMutableParameterCount() & {
		if (data_->queries.empty()) {
			throw cql::CqlLogicException(CQL_CODEINFO,
				"please call addQuery before addParameters");
		}
		auto& lastQuery = data_->queries.back();
		if (lastQuery.parameterSets.empty()) {
			lastQuery.parameterSets.emplace_back();
		}
		return lastQuery.parameterSets.back().first;
	}

	/** Get the mutable encoded parameters of the last parameter set */
	seastar::sstring& CqlBatchCommand::getMutableParameters() & {
		if (data_->queries.empty()) {
			throw cql::CqlLogicException(CQL_CODEINFO,
				"please call addQuery before addParameters");
		}
		auto& lastQuery = data_->queries.back();
		if (lastQuery.parameterSets.empty()) {
			lastQuery.parameterSets.emplace_back();
		}
		return lastQuery.parameterSets.back().second;
	}
}

