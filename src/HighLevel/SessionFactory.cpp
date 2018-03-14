#include <CQLDriver/HighLevel/SessionFactory.hpp>
#include "../LowLevel/Connection.hpp"
#include "./SessionData.hpp"

namespace cql {
	/** Defines members of SessionFactory */
	class SessionFactoryData {
	public:
		seastar::lw_shared_ptr<SessionConfiguration> sessionConfiguration;
		seastar::shared_ptr<NodeCollection> nodeCollection;
		seastar::lw_shared_ptr<ConnectionPool> connectionPool;

		SessionFactoryData(
			const SessionConfiguration& sessionConfigurationRef,
			const seastar::shared_ptr<NodeCollection>& nodeCollectionRef) :
			sessionConfiguration(seastar::make_lw_shared<
				SessionConfiguration>(sessionConfigurationRef)),
			nodeCollection(nodeCollectionRef),
			connectionPool(seastar::make_lw_shared<
				ConnectionPool>(sessionConfiguration, nodeCollection)) { }
	};

	/** Create a new session instance */
	Session SessionFactory::getSession() {
		return Session(makeObject<SessionData>(data_->connectionPool));
	}

	/** Constructor */
	SessionFactory::SessionFactory(
		const SessionConfiguration& sessionConfiguration,
		const seastar::shared_ptr<NodeCollection>& nodeCollection) :
		data_(seastar::make_shared<SessionFactoryData>(
			sessionConfiguration,
			nodeCollection)) { }
}

