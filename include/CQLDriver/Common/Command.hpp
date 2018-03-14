#pragma once
#include <cstdint>
#include <utility>
#include <chrono>
#include "./Utility/Object.hpp"
#include "./CommonDefinitions.hpp"
#include "./ColumnTrait.hpp"

namespace cql {
	/** Defines members of Command */
	class CommandData;

	/** Class represents a single cql command for query or execute */
	class Command {
	public:
		/** Check whether this is a valid command (will be false if moved) */
		bool isValid() const;

		/**
		 * Set the consistency level of this query
		 * For more information see this page:
		 * https://docs.datastax.com/en/cassandra/2.1/cassandra/dml/dml_config_consistency_c.html
		 */
		Command& setConsistency(ConsistencyLevel consistencyLevel) &;

		/** Set the consistency level of this query */
		Command&& setConsistency(ConsistencyLevel consistencyLevel) && {
			return std::move(setConsistency(consistencyLevel));
		}

		/**
		 * Set the page size of this query.
		 * You may also want to set the page state if you want to query the next page.
		 */
		Command& setPageSize(std::size_t pageSize) &;

		/** Set the page size of this query */
		Command&& setPageSize(std::size_t pageSize) && {
			return std::move(setPageSize(pageSize));
		}

		/**
		 * Set the paging state of this query.
		 * For the first page this is unnecessary.
		 * Please sure you called the setPageSize before this function.
		 */
		Command& setPagingState(std::string&& pagingState) &;

		/**
		 * Set the paging state of this query.
		 * For the first page this is unnecessary.
		 * Please sure you called the setPageSize before this function.
		 */
		Command&& setPagingState(std::string&& pagingState) && {
			return std::move(setPagingState(std::move(pagingState)));
		}

		/**
		 * Add single query parameter bound by position.
		 * The position is incremental, when this function is called.
		 */
		template <class T>
		Command& addParameter(T&& parameter) & {
			ColumnTrait<std::decay_t<T>>::encode(
				std::forward<T>(parameter), getParameters());
			++getParameterCount();
			return *this;
		}

		/** Add single query parameter bound by position */
		template <class T>
		Command&& addParameter(T&& parameter) && {
			return std::move(addParameter(std::forward<T>(parameter)));
		}

		/**
		 * Add multiple query parameters bound by position.
		 * The position is incremental, when this function is called.
		 */
		template <class... Args>
		Command& addParameters(Args&&... parameters) & {
			addParametersEncode(getParameters(), std::forward<Args>(parameters)...);
			getParameterCount() += sizeof...(Args);
			return *this;
		}

		/** Add multiple query parameters bound by position */
		template <class... Args>
		Command&& addParameters(Args&&... parameters) && {
			return std::move(addParameters(std::forward<Args>(parameters)...));
		}

		/**
		 * Set the serial consistency level of this query.
		 * Can only be either SERIAL or LOCAL_SERIAL.
		 */
		Command& setSerialConsistency(ConsistencyLevel consistencyLevel) &;

		/** Set the serial consistency level of this query */
		Command&& setSerialConsistency(ConsistencyLevel consistencyLevel) && {
			return std::move(setSerialConsistency(consistencyLevel));
		}

		/**
		 * Set the default timestamp of this query.
		 * This will replace the server side assigned timestamp as default timestamp.
		 * A timestamp in the query itself will still override this timestamp.
		 */
		Command& setDefaultTimestamp(std::chrono::system_clock::time_point timeStamp) &;

		/** Set the default timestamp of this query */
		Command&& setDefaultTimestamp(std::chrono::system_clock::time_point timeStamp) && {
			return std::move(setDefaultTimestamp(timeStamp));
		}

		/** Set the maximum retry times *after* the first try is failed */
		Command& setMaxRetries(std::size_t maxRetries) &;

		/** Set the maximum retry times *after* the first try is failed */
		Command&& setMaxRetries(std::size_t maxRetries) && {
			return std::move(setMaxRetries(maxRetries));
		}

		/** Get the query string of this query */
		std::pair<const char*, std::size_t> getQuery() const&;

		/** Get the consistency level of this query */
		ConsistencyLevel getConsistency() const;

		/** Get the page size of this query, the second value is false if is not set */
		const std::pair<std::size_t, bool>& getPageSize() const&;

		/** Get the paging state of this query */
		const std::string& getPagingState() const&;

		/** Get the count of parameters of this query */
		std::size_t getParameterCount() const&;

		/** Get the mutable count of parameters of this query */
		std::size_t& getParameterCount() &;

		/** Get the encoded parameters of this query */
		const std::string& getParameters() const&;

		/** Get the mutable encoded parameters of this query */
		std::string& getParameters() &;

		/**
		 * Get the serial consistency level of this query,
		 * the second value is false if is not set.
		 */
		const std::pair<ConsistencyLevel, bool>& getSerialConsistency() const&;

		/**
		 * Get the default timestamp of this query,
		 * the second value is false if is not set.
		 */
		const std::pair<std::chrono::system_clock::time_point, bool>& getDefaultTimestamp() const&;

		/** Get the maximum retry times *after* the first try is failed */
		std::size_t getMaxRetries() const;

		/** Constructor */
		explicit Command(std::string&& query);

		/** Constructor */
		Command(const char* query, std::size_t size);

		/** Constructor */
		template <std::size_t Size>
		explicit Command(const char(&query)[Size]) :
			Command(query, Size-1) {
			static_assert(Size > 0, "check size");
		}

		/** Constructor, build an empty(invalid) command */
		explicit Command(std::nullptr_t);

	private:
		/** Encode implementation of addParameters */
		static void addParametersEncode(std::string&) { }
		template <class Head, class... Rest>
		static void addParametersEncode(std::string& data, Head&& head, Rest&&... rest) {
			ColumnTrait<std::decay_t<Head>>::encode(std::forward<Head>(head), data);
			addParametersEncode(data, std::forward<Rest>(rest)...);
		}

	private:
		Object<CommandData> data_;
	};
}

