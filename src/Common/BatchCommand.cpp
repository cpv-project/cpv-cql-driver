#include <CQLDriver/Common/BatchCommand.hpp>
#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include "./BatchQueryData.hpp"

namespace cql {
	/** Defines members of BatchCommand */
	class BatchCommandData {
	public:
		BatchType batchType;
		std::optional<ConsistencyLevel> consistencyLevel;
		std::vector<BatchQueryData> queries;
		std::optional<ConsistencyLevel> serialConsistencyLevel;
		std::optional<std::chrono::system_clock::time_point> defaultTimestamp;
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
			consistencyLevel = {};
			queries.resize(0);
			serialConsistencyLevel = {};
			defaultTimestamp = {};
			maxRetries = 0;
		}
	};

	/** The storage of HttpRequestData */
	template <>
	thread_local ReusableStorageType<BatchCommandData>
		ReusableStorageInstance<BatchCommandData>;

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

	/** Set should prepare the last query */
	BatchCommand& BatchCommand::prepareQuery(bool value) & {
		if (CQL_UNLIKELY(data_->queries.empty())) {
			throw cql::LogicException(CQL_CODEINFO,
				"please call addQuery before prepareQuery");
		}
		data_->queries.back().needPrepare = value;
		return *this;
	}

	/** Open a new parameter set explicitly of the last query */
	BatchCommand& BatchCommand::openParameterSet() & {
		if (CQL_UNLIKELY(data_->queries.empty())) {
			throw cql::LogicException(CQL_CODEINFO,
				"please call addQuery before openParameterSet");
		}
		data_->queries.back().parameterSets.emplace_back();
		return *this;
	}

	/** Set the serial consistency level of this query */
	BatchCommand& BatchCommand::setSerialConsistency(
		ConsistencyLevel consistencyLevel) & {
		data_->serialConsistencyLevel = consistencyLevel;
		return *this;
	}

	/** Set the default timestamp of this query */
	BatchCommand& BatchCommand::setDefaultTimestamp(
		std::chrono::system_clock::time_point timeStamp) & {
		data_->defaultTimestamp = timeStamp;
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
	const std::optional<ConsistencyLevel>& BatchCommand::getConsistency() const& {
		return data_->consistencyLevel;
	}

	/** Get the queries in this batch */
	const std::vector<BatchQueryData>& BatchCommand::getQueries() const& {
		return data_->queries;
	}

	/** Get the mutable queries in this batch */
	std::vector<BatchQueryData>& BatchCommand::getQueries() & {
		return data_->queries;
	}

	/** Get the mutable count of parameters of the last parameter set */
	std::size_t& BatchCommand::getParameterCountOfLastSet() & {
		if (CQL_UNLIKELY(data_->queries.empty())) {
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
		if (CQL_UNLIKELY(data_->queries.empty())) {
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
	const std::optional<ConsistencyLevel>& BatchCommand::getSerialConsistency() const& {
		return data_->serialConsistencyLevel;
	}

	/** Get the default timestamp of this query */
	const std::optional<std::chrono::system_clock::time_point>&
		BatchCommand::getDefaultTimestamp() const& {
		return data_->defaultTimestamp;
	}

	/** Get the maximum retry times *after* the first try has failed */
	std::size_t BatchCommand::getMaxRetries() const {
		return data_->maxRetries;
	}

	/** Constructor */
	BatchCommand::BatchCommand() :
		data_(makeReusable<BatchCommandData>()) { }

	/** Constructor, build an empty(invalid) batch command */
	BatchCommand::BatchCommand(std::nullptr_t) :
		data_(nullptr) { }
}

