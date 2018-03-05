#pragma once
#include <cstdint>
#include <utility>
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
		/**
		 * Set the consistency level of this query
		 * For more information see this page:
		 * https://docs.datastax.com/en/cassandra/2.1/cassandra/dml/dml_config_consistency_c.html
		 */
		CqlCommand&& setConsistencyLevel(CqlConsistencyLevel consistencyLevel) &&;

		/**
		 * Set the page size of this query.
		 * You may also want to set the page state if you want to query the next page.
		 */
		CqlCommand&& setPageSize(std::size_t pageSize) &&;

		/**
		 * Set the page state of this query.
		 * For the first page this is unnecessary.
		 * Please sure you called the setPageSize before this function.
		 */
		CqlCommand&& setPageState(seastar::sstring&& pageState) &&;

		/**
		 * Add single query parameter bound by position.
		 * The position is incremental, when this function is called.
		 */
		template <class T>
		CqlCommand&& addParameter(T&& parameter) && {
			CqlColumnTrait<std::decay_t<T>>::encode(
				std::forward<T>(parameter), getMutableParameters());
			++getMutableParameterCount();
			return std::move(*this);
		}

		/**
		 * Add multiple query parameters bound by position.
		 * The position is incremental, when this function is called.
		 */
		template <class... Args>
		CqlCommand&& addParameters(Args&&... parameters) && {
			addParametersEncode(getMutableParameters(), std::forward<Args>(parameters)...);
			getMutableParameterCount() += sizeof...(Args);
			return std::move(*this);
		}

		/** Get the query string of this query */
		std::pair<const char*, std::size_t> getQuery() const&;

		/** Get the consistency level of this query */
		CqlConsistencyLevel getConsistencyLevel() const;

		/** Get the page size of this query, the second value is false if is not set */
		std::pair<std::size_t, bool> getPageSize() const;

		/** Get the page state of this query */
		const seastar::sstring& getPageState() const&;

		/** Get the count of parameters of this query */
		std::size_t getParameterCount() const;

		/** Get the encoded parameters of this query */
		const seastar::sstring& getParameters() const&;

		/** Constructor */
		CqlCommand(seastar::sstring&& query);

		/** Constructor */
		CqlCommand(const char* query, std::size_t size);

		/** Constructor */
		template <std::size_t Size>
		CqlCommand(const char(&query)[Size]) : CqlCommand(query, Size-1) {
			static_assert(Size > 0, "check size");
		}

	private:
		/** Get the mutable count of parameters of this query */
		std::size_t& getMutableParameterCount() &;

		/** Get the mutable encoded parameters of this query */
		seastar::sstring& getMutableParameters() &;

		/** Encode implementation of addParameters */
		void addParametersEncode(seastar::sstring&) { }
		template <class Head, class... Rest>
		void addParametersEncode(seastar::sstring& data, Head&& head, Rest&&... rest) {
			CqlColumnTrait<std::decay_t<Head>>::encode(std::forward<Head>(head), data);
			addParametersEncode(data, std::forward<Rest>(rest)...);
		}

	private:
		CqlObject<CqlCommandData> data_;
	};
}

