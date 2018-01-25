#pragma once
#include <core/shared_ptr.hh>
#include <core/future.hh>
#include <core/queue.hh>
#include <CqlDriver/Common/CqlSessionConfiguration.hpp>
#include "CqlConnectionStream.hpp"

namespace cql {
	class CqlConnection;
	class CqlNodeCollection;

	/** Class used to create and reuse database connections */
	class CqlConnectionPool :
		public seastar::enable_lw_shared_from_this<CqlConnectionPool> {
	public:
		/** Try to get a connection with idle stream, may return (nullptr, {}) if not available */
		std::pair<seastar::lw_shared_ptr<CqlConnection>, CqlConnectionStream> tryGetConnection();

		/** Get a connection with idle stream, wait until they are available */
		seastar::future<seastar::lw_shared_ptr<CqlConnection>, CqlConnectionStream> getConnection();

		/** Tell connection pool some connection become idle, this is optional */
		void notifyConnectionBecomeIdle(seastar::lw_shared_ptr<CqlConnection>&& connection);

		/** Constructor */
		CqlConnectionPool(
			const seastar::lw_shared_ptr<CqlSessionConfiguration>& sessionConfiguration,
			const seastar::shared_ptr<CqlNodeCollection>& nodeCollection);

		/** Disallow copy and move */
		CqlConnectionPool(const CqlConnectionPool&) = delete;
		CqlConnectionPool(CqlConnectionPool&&) = delete;
		CqlConnectionPool& operator=(const CqlConnectionPool&) = delete;
		CqlConnectionPool& operator=(CqlConnectionPool&&) = delete;

	private:
		/** Make a new ready-to-use connection and return it with an idle stream */
		seastar::future<seastar::lw_shared_ptr<CqlConnection>, CqlConnectionStream> makeConnection();

		/** Timer used to find idle connection and feed waiters */
		void findIdleConnectionTimer();

		/** Timer used to drop idle connections */
		void dropIdleConnectionTimer();

	private:
		seastar::lw_shared_ptr<CqlSessionConfiguration> sessionConfiguration_;
		seastar::shared_ptr<CqlNodeCollection> nodeCollection_;
		std::vector<seastar::lw_shared_ptr<CqlConnection>> allConnections_;
		seastar::queue<seastar::promise<
			seastar::lw_shared_ptr<CqlConnection>, CqlConnectionStream>> waiters_;
		bool findIdleConnectionTimerIsRunning_;
	};
}

