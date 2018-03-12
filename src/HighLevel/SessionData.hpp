#pragma once
#include <CQLDriver/Common/SessionConfiguration.hpp>
#include <CQLDriver/Common/NodeCollection.hpp>

namespace cql {
	/** Defines members of Session */
	class SessionData {
	public:
		seastar::shared_ptr<SessionConfiguration> sessionConfiguration;
		seastar::shared_ptr<NodeCollection> nodeCollection;

		SessionData() :
			sessionConfiguration(),
			nodeCollection() { }

		static void freeResources() { }

		void reset(
			const seastar::shared_ptr<SessionConfiguration>& sessionConfigurationRef,
			const seastar::shared_ptr<NodeCollection>& nodeCollectionRef) {
			sessionConfiguration = sessionConfigurationRef;
			nodeCollection = nodeCollectionRef;
		}
	};
}

