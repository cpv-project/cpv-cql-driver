#pragma once
#include <cstdint>
#include <utility>
#include <vector>
#include <optional>
#include "./Utility/Reusable.hpp"
#include "./CommonDefinitions.hpp"
#include "./ColumnTrait.hpp"

namespace cql {
	/** Defines members of BatchCommand */
	class BatchCommandData;

	/** Defines members of a single query in batch command */
	class BatchQueryData;

	/** Class represents multiple cql commands for execute */
	class BatchCommand {
	public:
		/** Check whether this is a valid command (will be false if moved) */
		bool isValid() const;

		/** Set the type of this batch, default is "Logged" */
		BatchCommand& setType(BatchType batchType) &;

		/** Set the type of this batch, default is "Logged" */
		BatchCommand&& setType(BatchType batchType) && {
			return std::move(setType(batchType));
		}

		/**
		 * Set the consistency level of this batch, default is "Quorum"
		 * For more information see this page:
		 * https://docs.datastax.com/en/cassandra/2.1/cassandra/dml/dml_config_consistency_c.html
		 */
		BatchCommand& setConsistency(ConsistencyLevel consistencyLevel) &;

		/** Set the consistency level of this batch, default is "Quorum" */
		BatchCommand&& setConsistency(ConsistencyLevel consistencyLevel) && {
			return std::move(setConsistency(consistencyLevel));
		}

		/** Add a new query to this batch */
		BatchCommand& addQuery(std::string&& query) &;

		/** Add a new query to this batch */
		BatchCommand&& addQuery(std::string&& query) && {
			return std::move(addQuery(std::move(query)));
		}

		/** Add a new query to this batch */
		BatchCommand& addQuery(const char* query, std::size_t size) &;

		/** Add a new query to this batch */
		BatchCommand&& addQuery(const char* query, std::size_t size) && {
			return std::move(addQuery(query, size));
		}

		/** Add a new query to this batch */
		template <std::size_t Size>
		BatchCommand& addQuery(const char(&query)[Size]) & {
			static_assert(Size > 0, "size of c string should not be 0");
			return addQuery(static_cast<const char*>(query), Size-1);
		}

		/** Add a new query to this batch */
		template <std::size_t Size>
		BatchCommand&& addQuery(const char(&query)[Size]) && {
			return std::move(addQuery<Size>(query));
		}

		/**
		 * Set should prepare the last query.
		 * This will override the default setting in SessionConfiguration.
		 * The prepare request will only be sent if the query isn't prepared before.
		 */
		BatchCommand& prepareQuery(bool value = true) &;

		/** Set should prepare the last query */
		BatchCommand&& prepareQuery(bool value = true) && {
			return std::move(prepareQuery(value));
		}

		/** Open a new parameter set explicitly of the last query */
		BatchCommand& openParameterSet() &;

		/** Open a new parameter set explicitly of the last query */
		BatchCommand&& openParameterSet() && {
			return std::move(openParameterSet());
		}

		/**
		 * Add single query parameter bound by position to the last parameter set.
		 * The position is incremental, when this function is called.
		 */
		template <class T>
		BatchCommand& addParameter(T&& parameter) & {
			ColumnTrait<std::decay_t<T>>::encode(
				std::forward<T>(parameter), getParametersOfLastSet());
			++getParameterCountOfLastSet();
			return *this;
		}

		/** Add single query parameter bound by position to the last parameter set */
		template <class T>
		BatchCommand&& addParameter(T&& parameter) && {
			return std::move(addParameter(std::forward<T>(parameter)));
		}

		/**
		 * Add multiple query parameters bound by position to the last parameter set.
		 * The position is incremental, when this function is called.
		 */
		template <class... Args>
		BatchCommand& addParameters(Args&&... parameters) & {
			addParametersEncode(
				getParametersOfLastSet(), std::forward<Args>(parameters)...);
			getParameterCountOfLastSet() += sizeof...(Args);
			return *this;
		}

		/** Add multiple query parameters bound by position to the last parameter set */
		template <class... Args>
		BatchCommand&& addParameters(Args&&... parameters) && {
			return std::move(addParameters(std::forward<Args>(parameters)...));
		}

		/**
		 * Set the serial consistency level of this query.
		 * Can only be either SERIAL or LOCAL_SERIAL.
		 */
		BatchCommand& setSerialConsistency(ConsistencyLevel consistencyLevel) &;

		/** Set the serial consistency level of this query */
		BatchCommand&& setSerialConsistency(ConsistencyLevel consistencyLevel) && {
			return std::move(setSerialConsistency(consistencyLevel));
		}

		/**
		 * Set the default timestamp of this query.
		 * This will replace the server side assigned timestamp as default timestamp.
		 * A timestamp in the query itself will still override this timestamp.
		 */
		BatchCommand& setDefaultTimestamp(std::chrono::system_clock::time_point timeStamp) &;

		/** Set the default timestamp of this query */
		BatchCommand&& setDefaultTimestamp(std::chrono::system_clock::time_point timeStamp) && {
			return std::move(setDefaultTimestamp(timeStamp));
		}

		/** Set the maximum retry times *after* the first try has failed, default is 0 */
		BatchCommand& setMaxRetries(std::size_t maxRetries) &;

		/** Set the maximum retry times *after* the first try has failed, default is 0 */
		BatchCommand&& setMaxRetries(std::size_t maxRetries) && {
			return std::move(setMaxRetries(maxRetries));
		}

		/** Get the type of this batch */
		BatchType getType() const;

		/** Get the consistency level of this batch */
		const std::optional<ConsistencyLevel>& getConsistency() const&;

		/** Get the queries in this batch */
		const std::vector<BatchQueryData>& getQueries() const&;

		/** Get the mutable queries in this batch */
		std::vector<BatchQueryData>& getQueries() &;

		/** Get the mutable count of parameters of the last parameter set */
		std::size_t& getParameterCountOfLastSet() &;

		/** Get the mutable encoded parameters of the last parameter set */
		std::string& getParametersOfLastSet() &;

		/**
		 * Get the serial consistency level of this query,
		 * the second value is false if is not set.
		 */
		const std::optional<ConsistencyLevel>& getSerialConsistency() const&;

		/**
		 * Get the default timestamp of this query,
		 * the second value is false if is not set.
		 */
		const std::optional<std::chrono::system_clock::time_point>& getDefaultTimestamp() const&;

		/** Get the maximum retry times *after* the first try is failed */
		std::size_t getMaxRetries() const;

		/** Constructor */
		BatchCommand();

		/** Constructor, build an empty(invalid) batch command */
		explicit BatchCommand(std::nullptr_t);

	private:
		/** Encode implementation of addParameters */
		static void addParametersEncode(std::string&) { }
		template <class Head, class... Rest>
		static void addParametersEncode(std::string& data, Head&& head, Rest&&... rest) {
			ColumnTrait<std::decay_t<Head>>::encode(std::forward<Head>(head), data);
			addParametersEncode(data, std::forward<Rest>(rest)...);
		}

	private:
		Reusable<BatchCommandData> data_;
	};
}

