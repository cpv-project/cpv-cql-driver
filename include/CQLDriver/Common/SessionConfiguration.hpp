#pragma once
#include <cstdint>
#include <chrono>
#include <memory>
#include "./Logger.hpp"

namespace cql {
	/** Defines members of SessionConfiguration */
	class SessionConfigurationData;

	/** The configuration of all sessions */
	class SessionConfiguration {
	public:
		/**
		 * Set the minimum database connection pool size.
		 * Notice it's per core configuration.
		 * The default value is 5.
		 */
		SessionConfiguration& setMinPoolSize(std::size_t minPoolSize);

		/**
		 * Set the maximum database connection pool size.
		 * Notice it's per core configuration,
		 * for example if the max pool size is 100 and there 2 cpu cores,
		 * then the maximum database connections in the pool for a single process is 200.
		 * The default value is 100.
		 */
		SessionConfiguration& setMaxPoolSize(std::size_t maxPoolSize);

		/**
		 * Set how many waiters can exist after connections are exhausted.
		 * After all connections are occupied and the poll can't raise new connection,
		 * the one wants to acquire connection from the pool has to wait,
		 * this set how many waiters can exist in such situation,
		 * if the maximum number of waitiers has been reached,
		 * the pool will return an error instead of waiting.
		 * Notice it's per core configuration.
		 * The default value is 100.
		 */
		SessionConfiguration& setMaxWaitersAfterConnectionsExhausted(std::size_t value);

		/**
		 * Set the dns cache time for the hostname of nodes.
		 * The default value is 30000ms.
		 */
		SessionConfiguration& setDnsCacheTime(std::chrono::milliseconds dnsCacheTime);

		/**
		 * Set the default keyspace of all connections created by this configuration.
		 * This can be overridden by specific keyspace in query, for example:
		 * "select * from my_keyspace.my_table"
		 */
		SessionConfiguration& setDefaultKeySpace(const std::string& keySpace);

		/**
		 * Set the default consistency level.
		 * The default value is "ConsistencyLevel.Quorum".
		 * This can be overridden by "setConsistency(...)" in Command and BatchCommand.
		 */
		SessionConfiguration& setDefaultConsistency(ConsistencyLevel consistency);

		/**
		 * Set should prepare all queries by default.
		 * The default value is false.
		 * This can be overridden by "prepareQuery(bool)" in Command and BatchCommand.
		 */
		SessionConfiguration& setPrepareAllQueries(bool value);

		/** Set the logger instance */
		SessionConfiguration& setLogger(const seastar::shared_ptr<Logger>& logger);

		/** Get the minimum database connection pool size */
		std::size_t getMinPoolSize() const;

		/** Get the maximum database connection pool size */
		std::size_t getMaxPoolSize() const;

		/** Get how many waitiers can exist after connections are exhausted */
		std::size_t getMaxWaitersAfterConnectionsExhausted() const;

		/** Get the dns cache time for the hostname of nodes */
		std::chrono::milliseconds getDnsCacheTime() const;

		/** Get the default keyspace of all connections created by this configuration */
		const std::string& getDefaultKeySpace() const&;

		/** Get the default consistency level */
		ConsistencyLevel getDefaultConsistency() const;

		/** Get should prepare all queries by default */
		bool getPrepareAllQueries() const;

		/** Get the logger instance */
		const seastar::shared_ptr<Logger>& getLogger() const&;

		/** Constructor */
		SessionConfiguration();

		/** Destructor */
		~SessionConfiguration();

		/** Copy and move */
		SessionConfiguration(const SessionConfiguration& other);
		SessionConfiguration(SessionConfiguration&& other);
		SessionConfiguration& operator=(const SessionConfiguration& other);
		SessionConfiguration& operator=(SessionConfiguration&& other);

	private:
		std::unique_ptr<SessionConfigurationData> data_;
	};
}

