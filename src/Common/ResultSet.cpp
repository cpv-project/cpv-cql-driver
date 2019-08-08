#include <CQLDriver/Common/ResultSet.hpp>
#include "./ResultSetData.hpp"

namespace cql {
	/** The storage of ResultSetData */
	template <>
	thread_local ReusableStorageType<ResultSetData>
		ReusableStorageInstance<ResultSetData>;

	/** Check whether this is a valid result set (will be false if moved) */
	bool ResultSet::isValid() const {
		return data_ != nullptr;
	}

	/** Get how many rows present in this result */
	// cppcheck-suppress unusedFunction
	std::size_t ResultSet::getRowsCount() const {
		return data_->rowsCount;
	}

	/** Get how many columns selected by the query that produced this result */
	std::size_t ResultSet::getColumnsCount() const {
		return data_->columnsCount;
	}

	/** Get the value used to retrieve the next page of results */
	const std::string& ResultSet::getPagingState() const& {
		return data_->pagingState;
	}

	/** Get the value used to retrieve the next page of results */
	std::string& ResultSet::getPagingState() & {
		return data_->pagingState;
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
	ResultSet::ResultSet(Reusable<ResultSetData>&& data) :
		data_(std::move(data)) { }

	/** Constructor, build an empty(invalid) result set */
	ResultSet::ResultSet(std::nullptr_t) :
		data_(nullptr) { }
}

