#pragma once
#include <cstdint>
#include <seastar/core/temporary_buffer.hh>
#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <CQLDriver/Common/CommonDefinitions.hpp>

namespace cql {
	/** Defines members of ResultSet */
	class ResultSetData {
	public:
		std::size_t rowsCount;
		std::size_t columnsCount;
		std::string pagingState;
		seastar::temporary_buffer<char> buffer;
		const char* decodePtr;
		const char* decodeEnd;

		ResultSetData() :
			rowsCount(0),
			columnsCount(0),
			pagingState(),
			buffer(),
			decodePtr(nullptr),
			decodeEnd(nullptr) { }

		void freeResources() {
			buffer = {};
			decodePtr = nullptr;
			decodeEnd = nullptr;
		}

		void reset(
			std::size_t rowsCountVal,
			std::size_t columnsCountVal,
			std::string&& pagingStateRef,
			seastar::temporary_buffer<char>&& bufferRef,
			std::size_t decodeFromOffset,
			std::size_t decodeToOffset) {
			rowsCount = rowsCountVal;
			columnsCount = columnsCountVal;
			pagingState = std::move(pagingStateRef);
			buffer = std::move(bufferRef);
			const char* ptr = buffer.begin();
			if (CQL_UNLIKELY(ptr == nullptr)) {
				throw LogicException(CQL_CODEINFO, "invalid buffer");
			}
			decodePtr = ptr + decodeFromOffset;
			decodeEnd = ptr + decodeToOffset;
			if (CQL_UNLIKELY(decodeEnd > buffer.end())) {
				freeResources();
				throw LogicException(CQL_CODEINFO, "invalid to offset");
			} else if (CQL_UNLIKELY(decodePtr > decodeEnd)) {
				freeResources();
				throw LogicException(CQL_CODEINFO, "invalid from offset");
			}
		}
	};
}

