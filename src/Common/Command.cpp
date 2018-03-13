#include <CQLDriver/Common/Command.hpp>

namespace cql {
	/** Defines members of Command */
	class CommandData {
	public:
		std::pair<const char*, std::size_t> queryCStr;
		std::string queryStr;
		ConsistencyLevel consistencyLevel;
		std::pair<std::size_t, bool> pageSize;
		std::string pagingState;
		std::size_t parameterCount;
		std::string parameters;
		std::pair<ConsistencyLevel, bool> serialConsistencyLevel;
		std::pair<std::chrono::system_clock::time_point, bool> defaultTimestamp;

		CommandData() :
			queryCStr(nullptr, 0),
			queryStr(),
			consistencyLevel(),
			pageSize(),
			pagingState(),
			parameterCount(),
			parameters(),
			serialConsistencyLevel(),
			defaultTimestamp() { }

		static void freeResources() { }

		void reset(const char* query, std::size_t size) {
			queryCStr = { query, size };
			queryStr.resize(0);
			resetExceptQuery();
		}

		void reset(std::string&& query) {
			queryCStr = { nullptr, 0 };
			queryStr = std::move(query);
			resetExceptQuery();
		}

	private:
		void resetExceptQuery() {
			consistencyLevel = ConsistencyLevel::Any;
			pageSize = { 0, false };
			pagingState.resize(0);
			parameterCount = 0;
			parameters.resize(0);
			serialConsistencyLevel = { ConsistencyLevel::Serial, false };
			defaultTimestamp = { {}, false };
		}
	};

	/** Check whether this is a valid command (will be false if moved) */
	bool Command::isValid() const {
		return data_ != nullptr;
	}

	/** Set the consistency level of this query */
	Command& Command::setConsistency(ConsistencyLevel consistencyLevel) & {
		data_->consistencyLevel = consistencyLevel;
		return *this;
	}

	/** Set the page size of this query */
	Command& Command::setPageSize(std::size_t pageSize) & {
		data_->pageSize = { pageSize, true };
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
		data_->serialConsistencyLevel = { consistencyLevel, true };
		return *this;
	}

	/** Set the default timestamp of this query */
	Command& Command::setDefaultTimestamp(
		std::chrono::system_clock::time_point timeStamp) & {
		data_->defaultTimestamp = { timeStamp, true };
		return *this;
	}

	/** Get the query string of this query */
	std::pair<const char*, std::size_t> Command::getQuery() const& {
		if (data_->queryCStr.first != nullptr) {
			return data_->queryCStr;
		} else {
			return { data_->queryStr.c_str(), data_->queryStr.size() };
		}
	}

	/** Get the consistency level of this query */
	ConsistencyLevel Command::getConsistency() const {
		return data_->consistencyLevel;
	}

	/** Get the page size of this query, the second value is false if is not set */
	const std::pair<std::size_t, bool>& Command::getPageSize() const& {
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
	const std::pair<ConsistencyLevel, bool>&
		Command::getSerialConsistency() const& {
		return data_->serialConsistencyLevel;
	}

	/** Get the default timestamp of this query */
	const std::pair<std::chrono::system_clock::time_point, bool>&
		Command::getDefaultTimestamp() const& {
		return data_->defaultTimestamp;
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

