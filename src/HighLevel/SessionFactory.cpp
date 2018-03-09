#include <CQLDriver/HighLevel/SessionFactory.hpp>
#include "./SessionFactoryImpl.hpp"

namespace cql {
	/** Create a default implementation of SessionFactory */
	seastar::shared_ptr<SessionFactory> SessionFactory::create(
		const SessionConfiguration& sessionConfiguration,
		const seastar::shared_ptr<NodeCollection>& nodeCollection) {
		return seastar::make_shared<SessionFactoryImpl>(sessionConfiguration, nodeCollection);
	}
}

