#include <CQLDriver/HighLevel/SessionFactory.hpp>
#include "./SessionData.hpp"

namespace cql {
	/** Defines members of SessionFactory */
	class SessionFactoryData {
	public:
		seastar::shared_ptr<SessionConfiguration> sessionConfiguration;
		seastar::shared_ptr<NodeCollection> nodeCollection;

		SessionFactoryData(
			const SessionConfiguration& sessionConfigurationRef,
			const seastar::shared_ptr<NodeCollection>& nodeCollectionRef) :
			sessionConfiguration(
				seastar::make_shared<SessionConfiguration>(sessionConfigurationRef)),
			nodeCollection(nodeCollectionRef) { }
	};

	/** Create a new session instance */
	Session SessionFactory::getSession() {
		return Session(makeObject<SessionData>(
			data_->sessionConfiguration,
			data_->nodeCollection));
	}

	/** Constructor */
	SessionFactory::SessionFactory(
		const SessionConfiguration& sessionConfiguration,
		const seastar::shared_ptr<NodeCollection>& nodeCollection) :
		data_(seastar::make_shared<SessionFactoryData>(
			sessionConfiguration,
			nodeCollection)) { }
}

