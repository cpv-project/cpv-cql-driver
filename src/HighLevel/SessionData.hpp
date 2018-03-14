#pragma once
#include <CQLDriver/Common/SessionConfiguration.hpp>
#include <CQLDriver/Common/NodeCollection.hpp>
#include "../LowLevel/ConnectionPool.hpp"

namespace cql {
	/** Defines members of Session */
	class SessionData {
	public:
		seastar::lw_shared_ptr<ConnectionPool> connectionPool;

		SessionData() : connectionPool() { }

		static void freeResources() { }

		void reset(const seastar::lw_shared_ptr<ConnectionPool>& connectionPoolRef) {
			connectionPool = connectionPoolRef;
		}
	};
}

