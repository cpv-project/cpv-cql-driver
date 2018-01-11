#include "CqlConnectorFactory.hpp"
#include "CqlSslConnector.hpp"
#include "CqlTcpConnector.hpp"

namespace cql {
	namespace CqlConnectorFactory {
		/** Get a suitable connector for specific node */
		seastar::shared_ptr<CqlConnectorBase> getConnector(
			const CqlNodeConfiguration& nodeConfiguration) {
			if (nodeConfiguration.getUseSsl()) {
				static thread_local seastar::shared_ptr<CqlSslConnector> connector =
					seastar::make_shared<CqlSslConnector>();
				return connector;
			} else {
				static thread_local seastar::shared_ptr<CqlTcpConnector> connector = 
					seastar::make_shared<CqlTcpConnector>();
				return connector;
			}
		}
	}
}

