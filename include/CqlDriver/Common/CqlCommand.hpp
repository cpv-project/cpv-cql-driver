#pragma once
#include <cstdint>
#include <utility>
#include <chrono>
#include <core/sstring.hh>
#include "./Utility/CqlObject.hpp"
#include "CqlCommonDefinitions.hpp"
#include "CqlColumnTrait.hpp"

namespace cql {
	/** Defines members of CqlCommand */
	class CqlCommandData;

	/** Class represents a single cql command for query or execute */
	class CqlCommand {
	public:
		/** Check whether this is a valid command (will be false if moved) */
		bool isValid() const;

		/**
		 * Set the consistency level of this query
		 * For more information see this page:
		 * https://docs.datastax.com/en/cassandra/2.1/cassandra/dml/dml_config_consistency_c.html
		 */
		CqlCommand& setConsistencyLevel(CqlConsistencyLevel consistencyLevel) &;

		/** Set the consistency level of this query */
		CqlCommand&& setConsistencyLevel(CqlConsistencyLevel consistencyLevel) && {
			return std::move(setConsistencyLevel(consistencyLevel));
		}

		/**
		 * Set the page size of this query.
		 * You may also want to set the page state if you want to query the next page.
		 */
		CqlCommand& setPageSize(std::size_t pageSize) &;

		/** Set the page size of this query */
		CqlCommand&& setPageSize(std::size_t pageSize) && {
			return std::move(setPageSize(pageSize));
		}

		/**
		 * Set the paging state of this query.
		 * For the first page this is unnecessary.
		 * Please sure you called the setPageSize before this function.
		 */
		CqlCommand& setPagingState(seastar::sstring&& pagingState) &;

		/**
		 * Set the paging state of this query.
		 * For the first page this is unnecessary.
		 * Please sure you called the setPageSize before this function.
		 */
		CqlCommand&& setPagingState(seastar::sstring&& pagingState) && {
			return std::move(setPagingState(std::move(pagingState)));
		}

		/**
		 * Add single query parameter bound by position.
		 * The position is incremental, when this function is called.
		 */
		template <class T>
		CqlCommand& addParameter(T&& parameter) & {
			CqlColumnTrait<std::decay_t<T>>::encode(
				std::forward<T>(parameter), getParameters());
			++getParameterCount();
			return *this;
		}

		/** Add single query parameter bound by position */
		template <class T>
		CqlCommand&& addParameter(T&& parameter) && {
			return std::move(addParameter(std::forward<T>(parameter)));
		}

		/**
		 * Add multiple query parameters bound by position.
		 * The position is incremental, when this function is called.
		 */
		template <class... Args>
		CqlCommand& addParameters(Args&&... parameters) & {
			addParametersEncode(getParameters(), std::forward<Args>(parameters)...);
			getParameterCount() += sizeof...(Args);
			return *this;
		}

		/** Add multiple query parameters bound by position */
		template <class... Args>
		CqlCommand&& addParameters(Args&&... parameters) && {
			return std::move(addParameters(std::forward<Args>(parameters)...));
		}

		/**
		 * Set the serial consistency level of this query.
		 * Can only be either SERIAL or LOCAL_SERIAL.
		 */
		CqlCommand& setSerialConsistencyLevel(CqlConsistencyLevel consistencyLevel) &;

		/** Set the serial consistency level of this query */
		CqlCommand&& setSerialConsistencyLevel(CqlConsistencyLevel consistencyLevel) && {
			return std::move(setSerialConsistencyLevel(consistencyLevel));
		}

		/**
		 * Set the default timestamp of this query.
		 * This will replace the server side assigned timestamp as default timestamp.
		 * A timestamp in the query itself will still override this timestamp.
		 */
		CqlCommand& setDefaultTimeStamp(std::chrono::system_clock::time_point timeStamp) &;

		/** Set the default timestamp of this query */
		CqlCommand&& setDefaultTimeStamp(std::chrono::system_clock::time_point timeStamp) && {
			return std::move(setDefaultTimeStamp(timeStamp));
		}

		/** Get the query string of this query */
		std::pair<const char*, std::size_t> getQuery() const&;

		/** Get the consistency level of this query */
		CqlConsistencyLevel getConsistencyLevel() const;

		/** Get the page size of this query, the second value is false if is not set */
		const std::pair<std::size_t, bool>& getPageSize() const&;

		/** Get the paging state of this query */
		const seastar::sstring& getPagingState() const&;

		/** Get the count of parameters of this query */
		std::size_t getParameterCount() const&;

		/** Get the mutable count of parameters of this query */
		std::size_t& getParameterCount() &;

		/** Get the encoded parameters of this query */
		const seastar::sstring& getParameters() const&;

		/** Get the mutable encoded parameters of this query */
		seastar::sstring& getParameters() &;

		/**
		 * Get the serial consistency level of this query,
		 * the second value is false if is not set.
		 */
		const std::pair<CqlConsistencyLevel, bool>& getSerialConsistencyLevel() const&;

		/**
		 * Get the default timestamp of this query,
		 * the second value is false if is not set.
		 */
		const std::pair<std::chrono::system_clock::time_point, bool>& getDefaultTimeStamp() const&;

		/** Constructor */
		explicit CqlCommand(seastar::sstring&& query);

		/** Constructor */
		CqlCommand(const char* query, std::size_t size);

		/** Constructor */
		template <std::size_t Size>
		explicit CqlCommand(const char(&query)[Size]) :
			CqlCommand(query, Size-1) {
			static_assert(Size > 0, "check size");
		}

	private:
		/** Encode implementation of addParameters */
		static void addParametersEncode(seastar::sstring&) { }
		template <class Head, class... Rest>
		static void addParametersEncode(seastar::sstring& data, Head&& head, Rest&&... rest) {
			CqlColumnTrait<std::decay_t<Head>>::encode(std::forward<Head>(head), data);
			addParametersEncode(data, std::forward<Rest>(rest)...);
		}

	private:
		CqlObject<CqlCommandData> data_;
	};
}

