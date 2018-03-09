#include "./ConnectorFactory.hpp"
#include "./SSLConnector.hpp"
#include "./TCPConnector.hpp"

namespace cql {
	namespace ConnectorFactory {
		/** Get a suitable connector for specific node */
		seastar::shared_ptr<ConnectorBase> getConnector(
			const NodeConfiguration& nodeConfiguration) {
			if (nodeConfiguration.getUseSSL()) {
				static thread_local seastar::shared_ptr<SSLConnector> connector =
					seastar::make_shared<SSLConnector>();
				return connector;
			} else {
				static thread_local seastar::shared_ptr<TCPConnector> connector = 
					seastar::make_shared<TCPConnector>();
				return connector;
			}
		}
	}
}

