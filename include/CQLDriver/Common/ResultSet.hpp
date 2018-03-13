#pragma once
#include <cstdint>
#include <type_traits>
#include "./Utility/Object.hpp"
#include "./ColumnTrait.hpp"

namespace cql {
	/** Defines members of ResultSet */
	class ResultSetData;

	/** Class use to store result of a query */
	class ResultSet {
	public:
		/** Check whether this is a valid result set (will be false if moved) */
		bool isValid() const;

		/** Get how many rows present in this result */
		std::size_t getRowsCount() const;

		/** Get how many columns selected by the query that produced this result */
		std::size_t getColumnsCount() const;

		/** Get the value used to retrieve the next page of results */
		const std::string& getPagingState() const&;
		std::string& getPagingState() &;

		// cppcheck-suppress functionConst
		/**
		 * Get the pointer use to decode contents of rows.
		 * Usually you should use fill instead of this function.
		 */
		const char*& getDecodePtr() &;

		/**
		 * Get the final pointer use to decode contents of rows.
		 * Usually you should use fill instead of this function.
		 */
		const char* getDecodeEnd() const;

		/**
		 * Fill the values with specificed types.
		 * The position is incremental, when this function is called.
		 * Notice it won't check the actual type is matched.
		 */
		template <class... Args>
		void fill(Args&... values) {
			fillDecode(getDecodePtr(), getDecodeEnd(), values...);
		}

		/** Constructor */
		explicit ResultSet(Object<ResultSetData>&& data);

		/** Constructor, build an empty(invalid) result set */
		explicit ResultSet(std::nullptr_t);

	private:
		/** Decode implementation of fill */
		static void fillDecode(const char*&, const char*) { }
		template <class Head, class... Rest>
		static void fillDecode(const char*& ptr, const char* end, Head& head, Rest&... rest) {
			ColumnTrait<std::decay_t<Head>>::decode(head, ptr, end);
			fillDecode(ptr, end, rest...);
		}

	private:
		Object<ResultSetData> data_;
	};
}

