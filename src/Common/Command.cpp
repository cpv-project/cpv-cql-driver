#include <CQLDriver/Common/Command.hpp>

namespace cql {
	/** Defines members of Command */
	class CommandData {
	public:
		StringHolder queryStr;
		std::optional<bool> needPrepare;
		std::optional<ConsistencyLevel> consistencyLevel;
		std::optional<std::size_t> pageSize;
		std::string pagingState;
		std::size_t parameterCount;
		std::string parameters;
		std::optional<ConsistencyLevel> serialConsistencyLevel;
		std::optional<std::chrono::system_clock::time_point> defaultTimestamp;
		std::size_t maxRetries;

		CommandData() :
			queryStr(),
			needPrepare(),
			consistencyLevel(),
			pageSize(),
			pagingState(),
			parameterCount(),
			parameters(),
			serialConsistencyLevel(),
			defaultTimestamp(),
			maxRetries() { }

		static void freeResources() { }

		void reset(const char* query, std::size_t size) {
			queryStr = StringHolder(query, size);
			resetExceptQuery();
		}

		void reset(std::string&& query) {
			queryStr = StringHolder(std::move(query));
			resetExceptQuery();
		}

	private:
		void resetExceptQuery() {
			needPrepare = {};
			consistencyLevel = {};
			pageSize = {};
			pagingState.resize(0);
			parameterCount = 0;
			parameters.resize(0);
			serialConsistencyLevel = {};
			defaultTimestamp = {};
			maxRetries = 0;
		}
	};

	/** Check whether this is a valid command (will be false if moved) */
	bool Command::isValid() const {
		return data_ != nullptr;
	}

	/** Set should prepare this query */
	Command& Command::prepareQuery(bool value) & {
		data_->needPrepare = value;
		return *this;
	}

	/** Set the consistency level of this batch, default is "Quorum" */
	Command& Command::setConsistency(ConsistencyLevel consistencyLevel) & {
		data_->consistencyLevel = consistencyLevel;
		return *this;
	}

	/** Set the page size of this query */
	Command& Command::setPageSize(std::size_t pageSize) & {
		data_->pageSize = pageSize;
		return *this;
	}

	/** Set the page state of this query */
	Command& Command::setPagingState(std::string&& pagingState) & {
		data_->pagingState = std::move(pagingState);
		return *this;
	}

	/** Set the serial consistency level of this query */
	Command& Command::setSerialConsistency(
		ConsistencyLevel consistencyLevel) & {
		data_->serialConsistencyLevel = consistencyLevel;
		return *this;
	}

	/** Set the default timestamp of this query */
	Command& Command::setDefaultTimestamp(
		std::chrono::system_clock::time_point timeStamp) & {
		data_->defaultTimestamp = timeStamp;
		return *this;
	}

	/** Set the maximum retry times *after* the first try is failed, default is 0 */
	Command& Command::setMaxRetries(std::size_t maxRetries) & {
		data_->maxRetries = maxRetries;
		return *this;
	}

	/** Get the query string of this query */
	std::string_view Command::getQuery() const& {
		return data_->queryStr.get();
	}

	/** Get should prepare this query */
	const std::optional<bool>& Command::getNeedPrepare() const& {
		return data_->needPrepare;
	}

	/** Get the consistency level of this query */
	const std::optional<ConsistencyLevel>& Command::getConsistency() const& {
		return data_->consistencyLevel;
	}

	/** Get the page size of this query, the second value is false if is not set */
	const std::optional<std::size_t>& Command::getPageSize() const& {
		return data_->pageSize;
	}

	/** Get the page state of this query */
	const std::string& Command::getPagingState() const& {
		return data_->pagingState;
	}

	/** Get the count of parameters of this query */
	std::size_t Command::getParameterCount() const& {
		return data_->parameterCount;
	}

	/** Get the mutable count of parameters of this query */
	std::size_t& Command::getParameterCount() & {
		return data_->parameterCount;
	}

	/** Get the encoded parameters of this query */
	const std::string& Command::getParameters() const& {
		return data_->parameters;
	}

	/** Get the mutable encoded parameters of this query */
	std::string& Command::getParameters() & {
		return data_->parameters;
	}

	/** Get the serial consistency level of this query */
	const std::optional<ConsistencyLevel>& Command::getSerialConsistency() const& {
		return data_->serialConsistencyLevel;
	}

	/** Get the default timestamp of this query */
	const std::optional<std::chrono::system_clock::time_point>&
		Command::getDefaultTimestamp() const& {
		return data_->defaultTimestamp;
	}

	/** Get the maximum retry times *after* the first try has failed */
	std::size_t Command::getMaxRetries() const {
		return data_->maxRetries;
	}

	/** Constructor */
	Command::Command(std::string&& query) :
		data_(makeObject<CommandData>(std::move(query))) { }

	/** Constructor */
	Command::Command(const char* query, std::size_t size) :
		data_(makeObject<CommandData>(query, size)) { }

	/** Constructor, build an empty(invalid) command */
	Command::Command(std::nullptr_t) :
		data_(nullptr) { }
}

