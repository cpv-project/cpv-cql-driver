#include <CQLDriver/Common/BatchCommand.hpp>
#include <CQLDriver/Common/Exceptions/LogicException.hpp>

namespace cql {
	/** Defines members of BatchCommand */
	class BatchCommandData {
	public:
		struct QueryData {
			std::pair<const char*, std::size_t> queryCStr;
			std::string queryStr;
			std::vector<std::pair<std::size_t, std::string>> parameterSets;

			QueryData() :
				queryCStr(),
				queryStr(),
				parameterSets() { }

			QueryData(const char* query, std::size_t size) :
				queryCStr(query, size),
				queryStr(),
				parameterSets() { }

			explicit QueryData(std::string&& query) :
				queryCStr(nullptr, 0),
				queryStr(std::move(query)),
				parameterSets() { }
		};

		BatchType batchType;
		ConsistencyLevel consistencyLevel;
		std::vector<QueryData> queries;
		std::pair<ConsistencyLevel, bool> serialConsistencyLevel;
		std::pair<std::chrono::system_clock::time_point, bool> defaultTimestamp;
		std::size_t maxRetries;

		BatchCommandData() :
			batchType(),
			consistencyLevel(),
			queries(),
			serialConsistencyLevel(),
			defaultTimestamp(),
			maxRetries() { }

		static void freeResources() { }

		void reset() {
			batchType = BatchType::Logged;
			consistencyLevel = ConsistencyLevel::Quorum;
			queries.resize(0);
			serialConsistencyLevel = { ConsistencyLevel::Serial, false };
			defaultTimestamp = { {}, false };
			maxRetries = 0;
		}
	};

	/** Check whether this is a valid command (will be false if moved) */
	bool BatchCommand::isValid() const {
		return data_ != nullptr;
	}

	/** Set the type of this batch, default is "Logged" */
	BatchCommand& BatchCommand::setType(BatchType batchType) & {
		data_->batchType = batchType;
		return *this;
	}

	/** Set the consistency level of this batch, default is "Quorum" */
	BatchCommand& BatchCommand::setConsistency(ConsistencyLevel consistencyLevel) & {
		data_->consistencyLevel = consistencyLevel;
		return *this;
	}

	/** Add a new query to this batch */
	BatchCommand& BatchCommand::addQuery(std::string&& query) & {
		data_->queries.emplace_back(std::move(query));
		return *this;
	}

	/** Add a new query to this batch */
	BatchCommand& BatchCommand::addQuery(const char* query, std::size_t size) & {
		data_->queries.emplace_back(query, size);
		return *this;
	}

	/** Open a new parameter set explicitly of the last query */
	BatchCommand& BatchCommand::openParameterSet() & {
		if (data_->queries.empty()) {
			throw cql::LogicException(CQL_CODEINFO,
				"please call addQuery before openParameterSet");
		}
		data_->queries.back().parameterSets.emplace_back();
		return *this;
	}

	/** Set the serial consistency level of this query */
	BatchCommand& BatchCommand::setSerialConsistency(
		ConsistencyLevel consistencyLevel) & {
		data_->serialConsistencyLevel = { consistencyLevel, true };
		return *this;
	}

	/** Set the default timestamp of this query */
	BatchCommand& BatchCommand::setDefaultTimestamp(
		std::chrono::system_clock::time_point timeStamp) & {
		data_->defaultTimestamp = { timeStamp, true };
		return *this;
	}

	/** Set the maximum retry times *after* the first try is failed, default is 0 */
	BatchCommand& BatchCommand::setMaxRetries(std::size_t maxRetries) & {
		data_->maxRetries = maxRetries;
		return *this;
	}

	/** Get the type of this batch */
	BatchType BatchCommand::getType() const {
		return data_->batchType;
	}

	/** Get the consistency level of this batch */
	ConsistencyLevel BatchCommand::getConsistency() const {
		return data_->consistencyLevel;
	}

	/** Get how many queries in this batch */
	std::size_t BatchCommand::getQueryCount() const {
		return data_->queries.size();
	}

	/** Get the query string by index */
	std::pair<const char*, std::size_t> BatchCommand::getQuery(std::size_t index) const& {
		auto& queryData = data_->queries.at(index);
		if (queryData.queryCStr.first != nullptr) {
			return queryData.queryCStr;
		} else {
			return { queryData.queryStr.c_str(), queryData.queryStr.size() };
		}
	}

	/** Get the parameter sets by index */
	const BatchCommand::ParameterSetsType&
		BatchCommand::getParameterSets(std::size_t index) const& {
		return data_->queries.at(index).parameterSets;
	}

	/** Get the mutable count of parameters of the last parameter set */
	std::size_t& BatchCommand::getParameterCountOfLastSet() & {
		if (data_->queries.empty()) {
			throw cql::LogicException(CQL_CODEINFO,
				"please call addQuery before addParameters");
		}
		auto& lastQuery = data_->queries.back();
		if (lastQuery.parameterSets.empty()) {
			lastQuery.parameterSets.emplace_back();
		}
		return lastQuery.parameterSets.back().first;
	}

	/** Get the mutable encoded parameters of the last parameter set */
	std::string& BatchCommand::getParametersOfLastSet() & {
		if (data_->queries.empty()) {
			throw cql::LogicException(CQL_CODEINFO,
				"please call addQuery before addParameters");
		}
		auto& lastQuery = data_->queries.back();
		if (lastQuery.parameterSets.empty()) {
			lastQuery.parameterSets.emplace_back();
		}
		return lastQuery.parameterSets.back().second;
	}

	/** Get the serial consistency level of this query */
	const std::pair<ConsistencyLevel, bool>&
		BatchCommand::getSerialConsistency() const& {
		return data_->serialConsistencyLevel;
	}

	/** Get the default timestamp of this query */
	const std::pair<std::chrono::system_clock::time_point, bool>&
		BatchCommand::getDefaultTimestamp() const& {
		return data_->defaultTimestamp;
	}

	/** Get the maximum retry times *after* the first try is failed */
	std::size_t BatchCommand::getMaxRetries() const {
		return data_->maxRetries;
	}

	/** Constructor */
	BatchCommand::BatchCommand() :
		data_(makeObject<BatchCommandData>()) { }

	/** Constructor, build an empty(invalid) batch command */
	BatchCommand::BatchCommand(std::nullptr_t) :
		data_(nullptr) { }
}

