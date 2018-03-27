#pragma once
#include <cstdint>
#include <utility>
#include <string>
#include <vector>
#include <optional>
#include <CQLDriver/Common/Utility/StringHolder.hpp>

namespace cql {
	/** Defines members of a single query in batch command */
	class BatchQueryData {
	public:
		StringHolder queryStr;
		std::optional<bool> needPrepare;
		std::vector<std::pair<std::size_t, std::string>> parameterSets;

		std::string_view getQuery() const& {
			return queryStr.get();
		}

		BatchQueryData() :
			queryStr(),
			needPrepare(),
			parameterSets() { }

		BatchQueryData(const char* query, std::size_t size) :
			queryStr(query, size),
			needPrepare(),
			parameterSets() { }

		explicit BatchQueryData(std::string&& query) :
			queryStr(std::move(query)),
			needPrepare(),
			parameterSets() { }
	};
}

