#include <CQLDriver/Common/ResultSet.hpp>
#include "./ResultSetData.hpp"

namespace cql {
	/** Check whether this is a valid result set (will be false if moved) */
	bool ResultSet::isValid() const {
		return data_ != nullptr;
	}

	/** Get how many rows present in this result */
	std::size_t ResultSet::getRowsCount() const {
		return data_->rowsCount;
	}

	/** Get how many columns selected by the query that produced this result */
	std::size_t ResultSet::getColumnsCount() const {
		return data_->columnsCount;
	}

	/** Get the pointer use to decode contents of rows */
	const char*& ResultSet::getDecodePtr() & {
		return data_->decodePtr;
	}

	/** Get the final pointer use to decode contents of rows */
	const char* ResultSet::getDecodeEnd() const {
		return data_->decodeEnd;
	}

	/** Constructor */
	ResultSet::ResultSet(Object<ResultSetData>&& data) :
		data_(std::move(data)) { }
}

