#pragma once
#include <cstdint>
#include <chrono>
#include <core/shared_ptr.hh>

namespace cql {
	/** Defines members of CqlSessionConfiguration */
	class CqlSessionConfigurationData;

	/** The configuration of all sessions */
	class CqlSessionConfiguration {
	public:
		/**
		 * Set the minimum database connection pool size.
		 * Notice it's per core configuration.
		 * The default value is 5.
		 */
		CqlSessionConfiguration& setMinPoolSize(std::size_t minPoolSize);

		/**
		 * Set the maximum database connection pool size.
		 * Notice it's per core configuration,
		 * for example if the max pool size is 100 and there 2 cpu cores,
		 * then the maximum database connections in the pool for single process is 200.
		 * The default value is 100.
		 */
		CqlSessionConfiguration& setMaxPoolSize(std::size_t maxPoolSize);

		/**
		 * Set how many waitiers can exist after connections are exhausted.
		 * After all connections are occupied and the poll can't raise new connection,
		 * the one wants to acquire connection from the pool has to wait,
		 * this set how many waiters can exist in such situation,
		 * if the maximum number of waitiers has been reached,
		 * the pool will return an error instead of waiting.
		 * Notice it's per core configuration.
		 * The default value is 100.
		 */
		CqlSessionConfiguration& setMaxWaitersAfterConnectionsExhausted(std::size_t value);

		/**
		 * Set the dns cache time for the hostname of nodes.
		 * The default value is 30000ms.
		 */
		CqlSessionConfiguration& setDnsCacheTime(std::chrono::milliseconds dnsCacheTime);

		/** Get the minimum database connection pool size */
		std::size_t getMinPoolSize() const;

		/** Get the maximum database connection pool size */
		std::size_t getMaxPoolSize() const;

		/** Get how many waitiers can exist after connections are exhausted */
		std::size_t getMaxWaitersAfterConnectionsExhausted() const;

		/** Get the dns cache time for the hostname of nodes */
		std::chrono::milliseconds getDnsCacheTime() const;

		/** Constructor */
		CqlSessionConfiguration();

	private:
		seastar::shared_ptr<CqlSessionConfigurationData> data_;
	};
}

