#include <CqlDriver/Common/CqlCommand.hpp>

namespace cql {
	/** Defines members of CqlCommand */
	class CqlCommandData {
	public:
		std::pair<const char*, std::size_t> queryCStr;
		seastar::sstring queryStr;
		CqlConsistencyLevel consistencyLevel;
		std::pair<std::size_t, bool> pageSize;
		seastar::sstring pagingState;
		std::size_t parameterCount;
		seastar::sstring parameters;
		std::pair<CqlConsistencyLevel, bool> serialConsistencyLevel;
		std::pair<std::chrono::system_clock::time_point, bool> defaultTimeStamp;

		CqlCommandData() :
			queryCStr(nullptr, 0),
			queryStr(),
			consistencyLevel(),
			pageSize(),
			pagingState(),
			parameterCount(),
			parameters(),
			serialConsistencyLevel(),
			defaultTimeStamp() { }

		// cppcheck-suppress functionStatic
		void freeResources() { }

		void reset(const char* query, std::size_t size) {
			queryCStr = { query, size };
			queryStr.resize(0);
			resetExceptQuery();
		}

		void reset(seastar::sstring&& query) {
			queryCStr = { nullptr, 0 };
			queryStr = std::move(query);
			resetExceptQuery();
		}

	private:
		void resetExceptQuery() {
			consistencyLevel = CqlConsistencyLevel::Any;
			pageSize = { 0, false };
			pagingState.resize(0);
			parameterCount = 0;
			parameters.resize(0);
			serialConsistencyLevel = { CqlConsistencyLevel::Serial, false };
			defaultTimeStamp = { {}, false };
		}
	};

	/** Check whether this is a valid command (will be false if moved) */
	bool CqlCommand::isValid() const {
		return data_ != nullptr;
	}

	/** Set the consistency level of this query */
	CqlCommand& CqlCommand::setConsistencyLevel(CqlConsistencyLevel consistencyLevel) & {
		data_->consistencyLevel = consistencyLevel;
		return *this;
	}

	/** Set the page size of this query */
	CqlCommand& CqlCommand::setPageSize(std::size_t pageSize) & {
		data_->pageSize = { pageSize, true };
		return *this;
	}

	/** Set the page state of this query */
	CqlCommand& CqlCommand::setPagingState(seastar::sstring&& pagingState) & {
		data_->pagingState = std::move(pagingState);
		return *this;
	}

	/** Set the serial consistency level of this query */
	CqlCommand& CqlCommand::setSerialConsistencyLevel(
		CqlConsistencyLevel consistencyLevel) & {
		data_->serialConsistencyLevel = { consistencyLevel, true };
		return *this;
	}

	/** Set the default timestamp of this query */
	CqlCommand& CqlCommand::setDefaultTimeStamp(
		std::chrono::system_clock::time_point timeStamp) & {
		data_->defaultTimeStamp = { timeStamp, true };
		return *this;
	}

	/** Get the query string of this query */
	std::pair<const char*, std::size_t> CqlCommand::getQuery() const& {
		if (data_->queryCStr.first != nullptr) {
			return data_->queryCStr;
		} else {
			return { data_->queryStr.c_str(), data_->queryStr.size() };
		}
	}

	/** Get the consistency level of this query */
	CqlConsistencyLevel CqlCommand::getConsistencyLevel() const {
		return data_->consistencyLevel;
	}

	/** Get the page size of this query, the second value is false if is not set */
	const std::pair<std::size_t, bool>& CqlCommand::getPageSize() const& {
		return data_->pageSize;
	}

	/** Get the page state of this query */
	const seastar::sstring& CqlCommand::getPagingState() const& {
		return data_->pagingState;
	}

	/** Get the count of parameters of this query */
	std::size_t CqlCommand::getParameterCount() const& {
		return data_->parameterCount;
	}

	/** Get the mutable count of parameters of this query */
	std::size_t& CqlCommand::getParameterCount() & {
		return data_->parameterCount;
	}

	/** Get the encoded parameters of this query */
	const seastar::sstring& CqlCommand::getParameters() const& {
		return data_->parameters;
	}

	/** Get the mutable encoded parameters of this query */
	seastar::sstring& CqlCommand::getParameters() & {
		return data_->parameters;
	}

	/** Get the serial consistency level of this query */
	const std::pair<CqlConsistencyLevel, bool>&
		CqlCommand::getSerialConsistencyLevel() const& {
		return data_->serialConsistencyLevel;
	}

	/** Get the default timestamp of this query */
	const std::pair<std::chrono::system_clock::time_point, bool>&
		CqlCommand::getDefaultTimeStamp() const& {
		return data_->defaultTimeStamp;
	}

	/** Constructor */
	CqlCommand::CqlCommand(seastar::sstring&& query) :
		data_(makeObject<CqlCommandData>(std::move(query))) { }

	/** Constructor */
	CqlCommand::CqlCommand(const char* query, std::size_t size) :
		data_(makeObject<CqlCommandData>(query, size)) { }

	/** Constructor, build an empty(invalid) command */
	CqlCommand::CqlCommand(std::nullptr_t) :
		data_(nullptr) { }
}

