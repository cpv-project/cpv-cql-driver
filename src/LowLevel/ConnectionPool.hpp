#pragma once
#include <seastar/core/shared_ptr.hh>
#include <seastar/core/future.hh>
#include <seastar/core/queue.hh>
#include <CQLDriver/Common/SessionConfiguration.hpp>
#include "../Common/MetricsData.hpp"
#include "./ConnectionStream.hpp"

namespace cql {
	class Connection;
	class NodeCollection;

	/** Class used to create and reuse database connections */
	class ConnectionPool :
		public seastar::enable_lw_shared_from_this<ConnectionPool> {
	public:
		/** Get the session configuration used by this pool */
		const SessionConfiguration& getSessionConfiguration() const& { return *sessionConfiguration_; }

		/** Get the metrics data */
		const MetricsData& getMetricsData() const& { return *metricsData_; }
		MetricsData& getMetricsData() & { return *metricsData_; }

		/** Try to get a connection with idle stream, may return (nullptr, {}) if not available */
		std::pair<seastar::lw_shared_ptr<Connection>, ConnectionStream> tryGetConnection();

		/** Get a connection with idle stream, wait until they are available */
		seastar::future<seastar::lw_shared_ptr<Connection>, ConnectionStream> getConnection();

		/** Return the connection to the pool manually, this is optional */
		void returnConnection(
			seastar::lw_shared_ptr<Connection>&& connection, ConnectionStream&& stream);

		/** Constructor */
		ConnectionPool(
			const seastar::lw_shared_ptr<SessionConfiguration>& sessionConfiguration,
			const seastar::shared_ptr<NodeCollection>& nodeCollection);

		/** Disallow copy and move */
		ConnectionPool(const ConnectionPool&) = delete;
		ConnectionPool(ConnectionPool&&) = delete;
		ConnectionPool& operator=(const ConnectionPool&) = delete;
		ConnectionPool& operator=(ConnectionPool&&) = delete;

	private:
		/** Spawn a new connection */
		void spawnConnection();

		/** Add a new waiter */
		seastar::future<seastar::lw_shared_ptr<Connection>, ConnectionStream> addWaiter();

		/** Find idle connection and feed waiters */
		void feedWaiters();

		/** Tell all waiters that an error has occurred */
		void cleanWaiters(const std::exception_ptr& ex);

		/** Timer used to find idle connection and feed waiters */
		void findIdleConnectionTimer();

		/** Timer used to drop idle connections */
		void dropIdleConnectionTimer();

	private:
		seastar::lw_shared_ptr<SessionConfiguration> sessionConfiguration_;
		seastar::shared_ptr<NodeCollection> nodeCollection_;
		seastar::lw_shared_ptr<MetricsData> metricsData_;
		std::vector<seastar::lw_shared_ptr<Connection>> allConnections_;
		std::size_t connectingCount_;
		seastar::queue<seastar::promise<
			seastar::lw_shared_ptr<Connection>, ConnectionStream>> waiters_;
		bool findIdleConnectionTimerIsRunning_;
		bool dropIdleConnectionTimerIsRunning_;
	};
}

