#pragma once
#include <CQLDriver/Common/Exceptions/LogicException.hpp>
#include <cstdint>
#include <core/temporary_buffer.hh>

namespace cql {
	/** Defines members of ResultSet */
	class ResultSetData {
	public:
		std::size_t rowsCount;
		std::size_t columnsCount;
		seastar::temporary_buffer<char> buffer;
		const char* decodePtr;
		const char* decodeEnd;

		ResultSetData() :
			rowsCount(0),
			columnsCount(0),
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
			seastar::temporary_buffer<char>&& bufferRef,
			std::size_t decodeFromOffset,
			std::size_t decodeToOffset) {
			rowsCount = rowsCountVal;
			columnsCount = columnsCountVal;
			buffer = std::move(bufferRef);
			const char* ptr = buffer.get();
			if (ptr == nullptr) {
				throw LogicException(CQL_CODEINFO, "invalid buffer");
			}
			decodePtr = ptr + decodeFromOffset;
			decodeEnd = ptr + decodeToOffset;
			if (decodeEnd > ptr + buffer.size()) {
				freeResources();
				throw LogicException(CQL_CODEINFO, "invalid to offset");
			} else if (decodePtr > decodeEnd) {
				freeResources();
				throw LogicException(CQL_CODEINFO, "invalid from offset");
			}
		}
	};
}

