#pragma once
#include <core/shared_ptr.hh>
#include <CqlDriver/Common/CqlNodeConfiguration.hpp>
#include "CqlConnectorBase.hpp"

namespace cql {
	namespace CqlConnectorFactory {
		/** Get a suitable connector for specific node */
		seastar::shared_ptr<CqlConnectorBase> getConnector(
			const CqlNodeConfiguration& nodeConfiguration);
	}
}

