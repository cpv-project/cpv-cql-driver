#pragma once
#include <cstdint>
#include <utility>
#include <string>
#include <vector>

namespace cql {
	/** Defines members of a single query in batch command */
	class BatchQueryData {
	public:
		std::pair<const char*, std::size_t> queryCStr;
		std::string queryStr;
		bool needPrepare;
		std::vector<std::pair<std::size_t, std::string>> parameterSets;

		std::pair<const char*, std::size_t> getQuery() const& {
			if (queryCStr.first != nullptr) {
				return queryCStr;
			} else {
				return { queryStr.c_str(), queryStr.size() };
			}
		}

		std::string getQueryAsString() const {
			auto query = getQuery();
			return std::string(query.first, query.second);
		}

		BatchQueryData() :
			queryCStr(nullptr, 0),
			queryStr(),
			needPrepare(false),
			parameterSets() { }

		BatchQueryData(const char* query, std::size_t size) :
			queryCStr(query, size),
			queryStr(),
			needPrepare(false),
			parameterSets() { }

		explicit BatchQueryData(std::string&& query) :
			queryCStr(nullptr, 0),
			queryStr(std::move(query)),
			needPrepare(false),
			parameterSets() { }
	};
}

