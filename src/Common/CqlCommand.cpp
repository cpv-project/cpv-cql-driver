#include <CqlDriver/Common/CqlCommand.hpp>

namespace cql {
	/** Defines members of CqlCommand */
	class CqlCommandData {
	public:
		std::pair<const char*, std::size_t> queryCStr;
		seastar::sstring queryStr;
		CqlConsistencyLevel consistencyLevel;
		std::pair<std::size_t, bool> pageSize;
		seastar::sstring pageState;
		std::size_t parameterCount;
		seastar::sstring parameters;

		CqlCommandData() :
			queryCStr(nullptr, 0),
			queryStr(),
			consistencyLevel(),
			pageSize(),
			pageState(),
			parameterCount(),
			parameters() { }

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
			pageState.resize(0);
			parameterCount = 0;
			parameters.resize(0);
		}
	};

	/** Set the consistency level of this query */
	CqlCommand&& CqlCommand::setConsistencyLevel(CqlConsistencyLevel consistencyLevel) && {
		data_->consistencyLevel = consistencyLevel;
		return std::move(*this);
	}

	/** Set the page size of this query */
	CqlCommand&& CqlCommand::setPageSize(std::size_t pageSize) && {
		data_->pageSize = { pageSize, true };
		return std::move(*this);
	}

	/** Set the page state of this query */
	CqlCommand&& CqlCommand::setPageState(seastar::sstring&& pageState) && {
		data_->pageState = std::move(pageState);
		return std::move(*this);
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
	std::pair<std::size_t, bool> CqlCommand::getPageSize() const {
		return data_->pageSize;
	}

	/** Get the page state of this query */
	const seastar::sstring& CqlCommand::getPageState() const& {
		return data_->pageState;
	}

	/** Get the count of parameters of this query */
	std::size_t CqlCommand::getParameterCount() const {
		return data_->parameterCount;
	}

	/** Get the encoded parameters of this query */
	const seastar::sstring& CqlCommand::getParameters() const& {
		return data_->parameters;
	}

	/** Constructor */
	CqlCommand::CqlCommand(seastar::sstring&& query) :
		data_(makeObject<CqlCommandData>(std::move(query))) { }

	/** Constructor */
	CqlCommand::CqlCommand(const char* query, std::size_t size) :
		data_(makeObject<CqlCommandData>(query, size)) { }

	/** Get the mutable count of parameters of this query */
	std::size_t& CqlCommand::getMutableParameterCount() & {
		return data_->parameterCount;
	}

	/** Get the mutable encoded parameters of this query */
	seastar::sstring& CqlCommand::getMutableParameters() & {
		return data_->parameters;
	}
}

