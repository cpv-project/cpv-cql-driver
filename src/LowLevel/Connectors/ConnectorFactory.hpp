#pragma once
#include <seastar/core/shared_ptr.hh>
#include <CQLDriver/Common/NodeConfiguration.hpp>
#include "./ConnectorBase.hpp"

namespace cql {
	namespace ConnectorFactory {
		/** Get a suitable connector for specific node */
		seastar::shared_ptr<ConnectorBase> getConnector(
			const NodeConfiguration& nodeConfiguration);
	}
}

