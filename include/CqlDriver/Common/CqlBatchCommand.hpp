#pragma once
#include <cstdint>
#include <utility>
#include <vector>
#include <core/sstring.hh>
#include "./Utility/CqlObject.hpp"
#include "CqlCommonDefinitions.hpp"
#include "CqlColumnTrait.hpp"

namespace cql {
	/** Defines members of CqlBatchCommand */
	class CqlBatchCommandData;

	/** Class represents multiple cql commands for execute */
	class CqlBatchCommand {
	public:
		/** Check whether this is a valid command (will be false if moved) */
		bool isValid() const;

		/**
		 * Set the consistency level of this batch
		 * For more information see this page:
		 * https://docs.datastax.com/en/cassandra/2.1/cassandra/dml/dml_config_consistency_c.html
		 */
		CqlBatchCommand& setConsistencyLevel(CqlConsistencyLevel consistencyLevel) &;

		/** Set the consistency level of this batch */
		CqlBatchCommand&& setConsistencyLevel(CqlConsistencyLevel consistencyLevel) && {
			return std::move(setConsistencyLevel(consistencyLevel));
		}

		/** Add a new query to this batch */
		CqlBatchCommand& addQuery(seastar::sstring&& query) &;

		/** Add a new query to this batch */
		CqlBatchCommand&& addQuery(seastar::sstring&& query) && {
			return std::move(addQuery(std::move(query)));
		}

		/** Add a new query to this batch */
		CqlBatchCommand& addQuery(const char* query, std::size_t size) &;

		/** Add a new query to this batch */
		CqlBatchCommand&& addQuery(const char* query, std::size_t size) && {
			return std::move(addQuery(query, size));
		}

		/** Add a new query to this batch */
		template <std::size_t Size>
		CqlBatchCommand& addQuery(const char(&query)[Size]) & {
			static_assert(Size > 0, "check size");
			return addQuery(static_cast<const char*>(query), Size-1);
		}

		/** Add a new query to this batch */
		template <std::size_t Size>
		CqlBatchCommand&& addQuery(const char(&query)[Size]) && {
			return std::move(addQuery<Size>(query));
		}

		/** Open a new parameter set explicitly of the last query */
		CqlBatchCommand& openParameterSet() &;

		/** Open a new parameter set explicitly of the last query */
		CqlBatchCommand&& openParameterSet() && {
			return std::move(openParameterSet());
		}

		/**
		 * Add single query parameter bound by position to the last parameter set.
		 * The position is incremental, when this function is called.
		 */
		template <class T>
		CqlBatchCommand& addParameter(T&& parameter) & {
			CqlColumnTrait<std::decay_t<T>>::encode(
				std::forward<T>(parameter), getParametersOfLastSet());
			++getParameterCountOfLastSet();
			return *this;
		}

		/** Add single query parameter bound by position to the last parameter set */
		template <class T>
		CqlBatchCommand&& addParameter(T&& parameter) && {
			return std::move(addParameter(std::forward<T>(parameter)));
		}

		/**
		 * Add multiple query parameters bound by position to the last parameter set.
		 * The position is incremental, when this function is called.
		 */
		template <class... Args>
		CqlBatchCommand& addParameters(Args&&... parameters) & {
			addParametersEncode(
				getParametersOfLastSet(), std::forward<Args>(parameters)...);
			getParameterCountOfLastSet() += sizeof...(Args);
			return *this;
		}

		/** Add multiple query parameters bound by position to the last parameter set */
		template <class... Args>
		CqlBatchCommand&& addParameters(Args&&... parameters) && {
			return std::move(addParameters(std::forward<Args>(parameters)...));
		}

		/**
		 * Set the serial consistency level of this query.
		 * Can only be either SERIAL or LOCAL_SERIAL.
		 */
		CqlBatchCommand& setSerialConsistencyLevel(CqlConsistencyLevel consistencyLevel) &;

		/** Set the serial consistency level of this query */
		CqlBatchCommand&& setSerialConsistencyLevel(CqlConsistencyLevel consistencyLevel) && {
			return std::move(setSerialConsistencyLevel(consistencyLevel));
		}

		/**
		 * Set the default timestamp of this query.
		 * This will replace the server side assigned timestamp as default timestamp.
		 * A timestamp in the query itself will still override this timestamp.
		 */
		CqlBatchCommand& setDefaultTimeStamp(std::chrono::system_clock::time_point timeStamp) &;

		/** Set the default timestamp of this query */
		CqlBatchCommand&& setDefaultTimeStamp(std::chrono::system_clock::time_point timeStamp) && {
			return std::move(setDefaultTimeStamp(timeStamp));
		}

		/** Get the consistency level of this batch */
		CqlConsistencyLevel getConsistencyLevel() const;

		/** Get how many queries in this batch */
		std::size_t getQueryCount() const;

		/** Get the query string by index */
		std::pair<const char*, std::size_t> getQuery(std::size_t index) const&;

		/** Get the parameter sets by index */
		using ParameterSetsType = std::vector<std::pair<std::size_t, seastar::sstring>>;
		const ParameterSetsType& getParameterSets(std::size_t index) const&;

		/** Get the mutable count of parameters of the last parameter set */
		std::size_t& getParameterCountOfLastSet() &;

		/** Get the mutable encoded parameters of the last parameter set */
		seastar::sstring& getParametersOfLastSet() &;

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
		CqlBatchCommand();

		/** Constructor, build an empty(invalid) batch command */
		explicit CqlBatchCommand(std::nullptr_t);

	private:
		/** Encode implementation of addParameters */
		static void addParametersEncode(seastar::sstring&) { }
		template <class Head, class... Rest>
		static void addParametersEncode(seastar::sstring& data, Head&& head, Rest&&... rest) {
			CqlColumnTrait<std::decay_t<Head>>::encode(std::forward<Head>(head), data);
			addParametersEncode(data, std::forward<Rest>(rest)...);
		}

	private:
		CqlObject<CqlBatchCommandData> data_;
	};
}

